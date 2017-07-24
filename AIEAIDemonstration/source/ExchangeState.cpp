#include "ExchangeState.h"
#include "Application2D.h"
#include "Transform.h"
#include "TradingAgent.h"

//called once per frame while the state is active
void ExchangeState::update(float deltaTime)
{
	//cast the base agent to it's true type
	TradingAgent* tradingAgent = (TradingAgent*)(FSM->agent);

	
}

//called when the state is entered into by the FSM holding it
void ExchangeState::onEnter(float deltaTime)
{
}

//called when the state is exited from by the FSM holding it
void ExchangeState::onExit(float deltaTime)
{
}
