#include "SearchState.h"
#include "Application2D.h"
#include "Transform.h"
#include "TradingAgent.h"

//called once per frame while the state is active
void SearchState::update(float deltaTime)
{

	//cast the base agent to it's true type
	TradingAgent* tradingAgent = (TradingAgent*)(FSM->agent);

	//transition case has occurred
	if (tradingAgent->trade != nullptr && tradingAgent->other != nullptr)
	{
		return;
	}

	//iterate through all currently posted trades
	for (size_t i = 0; i < tradingAgent->tradeRefs.size(); i++)
	{

		BlackboardReference* bref = tradingAgent->tradeRefs[i];

		//if the blackboard reference still references something
		if (bref->exists)
		{
			BlackboardData<Trade, Trade>* trade = (BlackboardData<Trade, Trade>*)bref->item;

			//iterate through all responses to the trade to see if any are currently valid trades
			for (size_t j = 0; j < trade->answers.size(); j++)
			{
				Trade* answer = &trade->answers[j];

				//is the responder still available for trading
				if (answer->responder->other == nullptr && answer->responder->trade == nullptr && answer->responder->compatibleTrade(answer))
				{
					tradingAgent->other = answer->responder;

					//iterate through all trade references again, setting the items to expire
					for (size_t k = 0; k < tradingAgent->tradeRefs.size(); k++)
					{
						if (tradingAgent->tradeRefs[k]->exists)
						{
							tradingAgent->tradeRefs[k]->item->timeRemaining = 0.0f;
						}
					}

					//iterate through all trade references in the other agent, setting their items to expire
					for (size_t k = 0; k < tradingAgent->other->tradeRefs.size(); k++)
					{
						if (tradingAgent->other->tradeRefs[k]->exists)
						{
							tradingAgent->other->tradeRefs[k]->item->timeRemaining = 0.0f;
						}
					}

					//set the other agent in the other agent to this agent
					answer->responder->other = tradingAgent;

					//set the responder to the original data
					trade->data.responder = answer->responder;

					//set the trade
					tradingAgent->trade = bref;
					tradingAgent->other->trade = bref;

					tradingAgent->initiator = true;
					tradingAgent->other->initiator = false;

					bref->item->timeLocked = true;

					//exit the state, transition ensure that the state won't be triggered again
					return;
				}
				else
				{
					//erase the response, it is no longer valid
					trade->answers.erase(trade->answers.begin() + j);
				}
			}
		}
		else
		{
			//remove the reference from the list and then delete it
			tradingAgent->tradeRefs.erase(tradingAgent->tradeRefs.begin() + i);
			delete bref;
		}
	}

	//iterate through all items on the blackboard, checking for a trade that this agent would like to make
	for (size_t i = 0; i < tradingAgent->tradingBlackboard->items.size(); i++)
	{
		BlackboardData<Trade, Trade>* item = (BlackboardData<Trade, Trade>*)tradingAgent->tradingBlackboard->items[i];

		//check if the agent can make the trade, and they didn't post it
		if (tradingAgent->compatibleTrade(&item->data) && item->data.poster != tradingAgent)
		{
			//check if another is already making this trade
			if (item->data.responder == nullptr)
			{
				//create a response trade and place it in the answers list
				Trade response = item->data;
				response.responder = tradingAgent;
			
				item->answers.push_back(response);
			}
		}
	}

	//post one trade if there is room for one
	if (tradingAgent->tradeRefs.size() < tradingAgent->maxTrades)
	{
		postRandomTrade(tradingAgent->itemsLostDuringTrades, tradingAgent->itemsGainedDuringTrades);
	}

}

//called when the state is entered into by the FSM holding it
void SearchState::onEnter(float deltaTime)
{
}

//called when the state is exited from by the FSM holding it
void SearchState::onExit(float deltaTime)
{
	//cast the base agent to it's true type
	TradingAgent* tradingAgent = (TradingAgent*)(FSM->agent);

	//set the target to the other agent to trade with
	tradingAgent->targetVector = &tradingAgent->other->transform->translation;
}

//generates a random trade to post to the blackboard
void SearchState::postRandomTrade(unsigned int itemsLost, unsigned int itemsGained)
{
	//cast the base agent to it's true type
	TradingAgent* tradingAgent = (TradingAgent*)(FSM->agent);

	//if there is any room on the blackboard for another message
	if (tradingAgent->tradingBlackboard->availableIds.size() > 0)
	{
		std::vector<ObjectType> invCopy = tradingAgent->inventory;
		std::vector<ObjectType> give;

		//take 'itemsLost' items
		for (unsigned int i = 0; i < itemsLost; i++)
		{
			//generate a random index for the array
			int randNum = rand() % invCopy.size();

			//add the item to the list to give
			give.push_back(invCopy[randNum]);

			//erase the item from the temp inventory copy
			invCopy.erase(invCopy.begin() + randNum);
		}

		std::vector<ObjectType> recieve;

		//create 'itemsGained' random items
		for (unsigned int i = 0; i < itemsGained; i++)
		{
			//generate a random index for the array
			int randNum = rand() % OBJECT_SIZE;

			//add the item to the list to give
			recieve.push_back((ObjectType)randNum);
		}

		//register the trade
		BlackboardReference* bref = tradingAgent->tradingBlackboard->registerItemWithReference<Trade, Trade>(tradingAgent->tradeExpiryTime);
		
		((BlackboardData<Trade, Trade>*)bref->item)->data.give = give;
		((BlackboardData<Trade, Trade>*)bref->item)->data.recieve = recieve;
		((BlackboardData<Trade, Trade>*)bref->item)->data.poster = tradingAgent;

		//add the trade to the trade references list
		tradingAgent->tradeRefs.push_back(bref);
	}
}

//checks a trade to see if this agent can perform it
bool TradingAgent::compatibleTrade(Trade * trade)
{
	//cast the base agent to it's true type
	TradingAgent* tradingAgent = (TradingAgent*)(FSM->agent);

	std::vector<ObjectType> invCopy = tradingAgent->inventory;

	//iterate through all the items aiming to be given
	for (size_t i = 0; i < trade->recieve.size(); i++)
	{
		ObjectType expected = trade->recieve[i];

		bool match = false;

		//search for a match for expected in the inventory copy
		for (size_t j = 0; j < invCopy.size(); j++)
		{
			//if there is an item that matches the expected item in the list, remove it and start again
			if (invCopy[j] == expected)
			{
				//erase the inventory copy
				invCopy.erase(invCopy.begin() + j);
				match = true;
				break;
			}
		}

		if (!match)
		{
			return false;
		}
	}

	//matches were found for every item in the trade
	return true;
}
