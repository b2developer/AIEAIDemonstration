#include "GotoState.h"
#include "Application2D.h"
#include "Transform.h"
#include "TradingAgent.h"

//called once per frame while the state is active
void GotoState::update(float deltaTime)
{

	//cast the base agent to it's true type
	TradingAgent* tradingAgent = (TradingAgent*)(FSM->agent);

	Vector2 relative = (*tradingAgent->targetVector - tradingAgent->transform->translation).normalised();

	//move towards the target
	tradingAgent->transform->translation += relative * tradingAgent->movementSpeed * deltaTime;
}

//called when the state is entered into by the FSM holding it
void GotoState::onEnter(float deltaTime)
{
}

//called when the state is exited from by the FSM holding it
void GotoState::onExit(float deltaTime)
{
}
