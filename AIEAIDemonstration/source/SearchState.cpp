#include "SearchState.h"
#include "Application2D.h"
#include "Transform.h"
#include "TradingAgent.h"

//called once per frame while the state is active
void SearchState::update(float deltaTime)
{

	//cast the base agent to it's true type
	TradingAgent* tradingAgent = (TradingAgent*)(FSM->agent);

	//another agent has accepted a trade with this agent
	if (tradingAgent->currentData != nullptr)
	{
		return;
	}

	//iterate through all the proposed trades, checking each one for expiry
	for (size_t i = 0; i < tradingAgent->proposedTrades.size(); i++)
	{

		//store the trade and id iterated to in a temp varaible (improves readability)
		int id = tradingAgent->proposedIds[i];

		//search for the current item in the proposed trades on the blackboard
		BlackboardItem* searchTest = tradingAgent->tradingBlackboard->searchFor<Trade, Trade>(id);

		//if the item stored in the agent doesn't exist on the blackboard anymore, remove it
		if (searchTest == nullptr)
		{
			//remove the trade link and the id link
			tradingAgent->proposedTrades.erase(tradingAgent->proposedTrades.begin() + i);
			tradingAgent->proposedIds.erase(tradingAgent->proposedIds.begin() + i);
		}
	}

	//iterate through all items in the blackboard, checking for a trade that the agent can perform
	for (size_t i = 0; i < tradingAgent->tradingBlackboard->items.size(); i++)
	{
		BlackboardItem* data = tradingAgent->tradingBlackboard->items[i];

		BlackboardData<Trade, Trade>* tradeItem = dynamic_cast<BlackboardData<Trade, Trade>*>(data);

		//check that the blackboard item is a trading post
		if (tradeItem != nullptr)
		{
			bool alreadyResponded = false;

			//check that the agent didn't already respond to the trade post
			for (size_t i = 0; i < tradeItem->answers.size(); i++)
			{
				Trade response = tradeItem->answers[i];

				//if a response was already made, set a flag so that the agent won't make another
				if (response.responder == tradingAgent)
				{
					alreadyResponded = true;
					break;
				}
			}

			//can a trade be performed with this agent (and this agent didnt post it or respond to it already!?)
			if (tradeItem->data.poster != tradingAgent && !alreadyResponded && compatibleTrade(&tradeItem->data))
			{
				//respond to trade
				Trade response = Trade();
				response = tradeItem->data;
				response.responder = tradingAgent;

				tradeItem->answers.push_back(response);
			}
		}

	}

	//if the agent is allowed to propose another trade
	if (tradingAgent->maxTrades > tradingAgent->proposedTrades.size())
	{
		postRandomTrade(tradingAgent->itemsLostDuringTrades, tradingAgent->itemsGainedDuringTrades);
	}

	//check current proposed trades to see if responses have been made
	for (size_t i = 0; i < tradingAgent->proposedTrades.size(); i++)
	{
		//search for the current item in the proposed trades on the blackboard
		BlackboardData<Trade, Trade>* postedTrade = tradingAgent->proposedTrades[i];

		//iterate through all answers to the trade, checking each agent's availability (they aren't trading already)
		for (size_t j = 0; j < postedTrade->answers.size(); j++)
		{
			Trade* response = &postedTrade->answers[j];

			//if the responder isn't trading already trading
			if (response->responder->currentData == nullptr)
			{
				//remove all trades, this trade might change what's available
				for (size_t k = 0; k < tradingAgent->proposedTrades.size(); k++)
				{
					BlackboardData<Trade, Trade>* postedTrade2 = tradingAgent->proposedTrades[k];

					postedTrade2->timeLocked = false;
					postedTrade2->timeRemaining = 0.0f;
				}

				//remove all proposal messages
				tradingAgent->proposedTrades.clear();
				tradingAgent->proposedIds.clear();

				//organise a transition for both agents into the next state
				tradingAgent->initiator = true;
				tradingAgent->target = response->responder;

				//lock the trade item on the blackboard
				tradingAgent->currentData = postedTrade;
				tradingAgent->currentData->data = *response;
				tradingAgent->currentData->timeLocked = true;
				

				//other agent

				//remove all trades, this trade might change what's available
				for (size_t k = 0; k < response->responder->proposedTrades.size(); k++)
				{
					BlackboardData<Trade, Trade>* postedTrade2 = response->responder->proposedTrades[k];

					postedTrade2->timeLocked = false;
					postedTrade2->timeRemaining = 0.0f;
				}

				//remove all proposal messages
				response->responder->proposedTrades.clear();
				response->responder->proposedIds.clear();

				response->responder->initiator = false;
				response->responder->target = tradingAgent;
				response->responder->currentData = postedTrade;
				response->responder->currentData->data = *response;

				//a transition will occur, exit the function
				return;
			}
			else
			{
				//erase the response, the responder isn't looking for a trade anymore
				postedTrade->answers.erase(postedTrade->answers.begin() + j);
			}
		}
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
	tradingAgent->targetVector = &tradingAgent->target->transform->translation;
}

//generates a random trade to post to the blackboard
void SearchState::postRandomTrade(unsigned int itemsLost, unsigned int itemsGained)
{
	//cast the base agent to it's true type
	TradingAgent* tradingAgent = (TradingAgent*)(FSM->agent);

	//remember what hasn't been removed from the inventory
	std::vector<ObjectType> inventoryCopy = tradingAgent->inventory;

	std::vector<ObjectType> lostItems;

	//pick random items from inventory to lose
	for (unsigned int i = 0; i < itemsLost; i++)
	{
		//check if the inventory copy still has items left in it
		if (inventoryCopy.size() == 0)
		{
			break;
		}

		//erase a random item in the inventory copy
		int randomNum = rand() % inventoryCopy.size();
		lostItems.push_back(inventoryCopy[randomNum]);
		inventoryCopy.erase(inventoryCopy.begin() + randomNum);
	}

	std::vector<ObjectType> gainedItems;

	//pick random items from all inventory to gain
	for (unsigned int i = 0; i < itemsGained; i++)
	{
		gainedItems.push_back((ObjectType)(rand() % OBJECT_SIZE));
	}

	//get the new item's id
	int newId = tradingAgent->tradingBlackboard->registerItem<Trade, Trade>(TRADE_LIFETIME);

	BlackboardData<Trade, Trade>* item = tradingAgent->tradingBlackboard->searchFor<Trade, Trade>(newId);

	//make a trade object from the random selections
	item->data.poster = tradingAgent;
	item->data.give = lostItems;
	item->data.recieve = gainedItems;
	item->data.responder = nullptr;

	tradingAgent->proposedTrades.push_back(item);
	tradingAgent->proposedIds.push_back(newId);
}

//checks a trade to see if this agent can perform it
bool SearchState::compatibleTrade(Trade * trade)
{

	//cast the base agent to it's true type
	TradingAgent* tradingAgent = (TradingAgent*)(FSM->agent);

	std::vector<ObjectType> inventoryCopy = tradingAgent->inventory;

	//iterate through the items in the trade that would be given by this agent
	for (size_t i = 0; i < trade->recieve.size(); i++)
	{
		//store in a temp variable for speed (not indexing multiple times) and readability
		ObjectType object = trade->recieve[i];

		bool copyFound = false;

		//iterate through the inventory copy, searching for a duplicate item
		for (size_t j = 0; j < inventoryCopy.size(); j++)
		{
			//if the item matches, erase it from the inventory copy so it won't be found again
			if (inventoryCopy[j] == object)
			{
				copyFound = true;
				inventoryCopy.erase(inventoryCopy.begin() + j);

				break;
			}
		}

		//if a copy was found, remove it from the inventory copy
		if (!copyFound)
		{
			return false;
		}
	}

	return true;
}
