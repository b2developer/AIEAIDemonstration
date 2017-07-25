#include "ExchangeState.h"
#include "Application2D.h"
#include "Transform.h"
#include "TradingAgent.h"

//called once per frame while the state is active
void ExchangeState::update(float deltaTime)
{
	//cast the base agent to it's true type
	TradingAgent* tradingAgent = (TradingAgent*)(FSM->agent);

	//exit if the trade no longer exists
	if (tradingAgent->trade == nullptr)
	{
		return;
	}

	Trade* trade = &((BlackboardData<Trade, Trade>*)tradingAgent->trade->item)->data;

	if (tradingAgent->initiator)
	{
		//exit the state if this agent is waiting for the other to give it's items
		if (trade->give.size() == 0)
		{
			return;
		}
	}
	else
	{
		//exit the state if this agent is waiting for the other to give it's items
		if (trade->recieve.size() == 0)
		{
			return;
		}
	}

	//count-down the timer
	tradingAgent->exchangeTimer -= deltaTime;

	//if the timer has counted-down
	if (tradingAgent->exchangeTimer < 0.0f)
	{
		//reset the timer
		tradingAgent->exchangeTimer = tradingAgent->exchangeDuration;

		if (tradingAgent->initiator)
		{
			ObjectType next = trade->give[0];

			//search for the correct item to remove and give to the other agent
			for (size_t i = 0; i < tradingAgent->inventory.size(); i++)
			{
				//if the inventory item matches the next item to give
				if (tradingAgent->inventory[i] == next)
				{
					//remove it from this agent's inventory and add it to the other agent's
					tradingAgent->inventory.erase(tradingAgent->inventory.begin() + i);
					tradingAgent->other->inventory.push_back(next);

					trade->give.erase(trade->give.begin());
					break;
				}
			}
		}
		else
		{
			ObjectType next = trade->recieve[0];

			//search for the correct item to remove and give to the other agent
			for (size_t i = 0; i < tradingAgent->inventory.size(); i++)
			{
				//if the inventory item matches the next item to give
				if (tradingAgent->inventory[i] == next)
				{
					//remove it from this agent's inventory and add it to the other agent's
					tradingAgent->inventory.erase(tradingAgent->inventory.begin() + i);
					tradingAgent->other->inventory.push_back(next);

					trade->recieve.erase(trade->recieve.begin());
					break;
				}
			}
		}

	}

}

//called when the state is entered into by the FSM holding it
void ExchangeState::onEnter(float deltaTime)
{
}

//called when the state is exited from by the FSM holding it
void ExchangeState::onExit(float deltaTime)
{
	//cast the base agent to it's true type
	TradingAgent* tradingAgent = (TradingAgent*)(FSM->agent);
	
	//safe delete of the coupled object
	if (tradingAgent->trade != nullptr)
	{

		//force the item to expire
		tradingAgent->trade->item->timeLocked = false;
		tradingAgent->trade->item->timeRemaining = 0.0f;

		tradingAgent->trade = nullptr;
		tradingAgent->other->trade = nullptr;
	}

	tradingAgent->other = nullptr;
	tradingAgent->targetVector = &tradingAgent->origin;
}
