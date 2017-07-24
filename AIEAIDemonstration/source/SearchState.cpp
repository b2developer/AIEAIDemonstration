#include "SearchState.h"
#include "Application2D.h"
#include "Transform.h"
#include "TradingAgent.h"

//called once per frame while the state is active
void SearchState::update(float deltaTime)
{

	//cast the base agent to it's true type
	TradingAgent* tradingAgent = (TradingAgent*)(FSM->agent);

	

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
	
}

//checks a trade to see if this agent can perform it
bool SearchState::compatibleTrade(Trade * trade)
{

	return false;
}
