#include "SearchState.h"
#include "Application2D.h"
#include "TradingAgent.h"

//called once per frame while the state is active
void SearchState::update(float deltaTime)
{
	//cast the base agent to it's true type
	TradingAgent* tradingAgent = (TradingAgent*)(FSM->agent);

	//iterate through all the proposed trades, checking each one for expiry
	for (size_t i = 0; i < tradingAgent->proposedTrades.size(); i++)
	{

		//store the trade and id iterated to in a temp varaible (improves readability)
		Trade* trade = tradingAgent->proposedTrades[i];
		int id = tradingAgent->proposedIds[i];

		//search for the current item in the proposed trades on the blackboard
		BlackboardItem* searchTest = tradingAgent->tradingBlackboard->searchFor<Trade*, Trade*>(id);

		//if the item stored in the agent doesn't exist on the blackboard anymore, remove it
		if (searchTest == nullptr)
		{
			//remove the trade link and the id link
			tradingAgent->proposedTrades.erase(tradingAgent->proposedTrades.begin() + i);
			tradingAgent->proposedIds.erase(tradingAgent->proposedIds.begin() + i);
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
	for (int i = 0; i < itemsLost; i++)
	{
		//check if the inventory copy still has items left in it
		if (inventoryCopy.size() == 0)
		{
			break;
		}

		//erase a random item in the inventory copy
		int randomNum = rand() % inventoryCopy.size();
		lostItems.push_back(inventoryCopy[i]);
		inventoryCopy.erase(inventoryCopy.begin() + i);
	}

	std::vector<ObjectType> gainedItems;

	//pick random items from all inventory to gain
	for (int i = 0; i < itemsGained; i++)
	{
		gainedItems.push_back((ObjectType)(rand() % OBJECT_SIZE));
	}

	int newId = tradingAgent->tradingBlackboard->registerItem<Trade*, Trade*>(TRADE_LIFETIME);

	BlackboardData<Trade*, Trade*>* item = tradingAgent->tradingBlackboard->searchFor<Trade*, Trade*>(newId);

	//make a trade object from the random selections
	item->data->poster = tradingAgent;
	item->data->give = lostItems;
	item->data->recieve = gainedItems;
	item->data->responder = nullptr;
}

//checks a trade if it
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
		for (std::vector<ObjectType>::iterator iter = inventoryCopy.begin(); iter < inventoryCopy.end(); iter++)
		{
			//if the item matches, break out of the loop
			if (*iter == object)
			{
				copyFound = true;
				inventoryCopy.erase(iter);
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
