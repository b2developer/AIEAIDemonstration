#include "ExchangeState.h"
#include "Application2D.h"
#include "Transform.h"
#include "TradingAgent.h"

//called once per frame while the state is active
void ExchangeState::update(float deltaTime)
{

	//cast the base agent to it's true type
	TradingAgent* tradingAgent = (TradingAgent*)(FSM->agent);

	Trade* trade = &tradingAgent->currentData->data;

	if (!tradingAgent->currentData)
	{
		return;
	}

	trade->give.clear();
	trade->recieve.clear();
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

	if (!tradingAgent->initiator)
	{
		tradingAgent->target->currentData = nullptr;
		tradingAgent->currentData->timeLocked = false;
		tradingAgent->currentData->timeRemaining = 0.0f;
	}
	tradingAgent->currentData = nullptr;

	//set the target to home
	tradingAgent->targetVector = &tradingAgent->origin;
}
