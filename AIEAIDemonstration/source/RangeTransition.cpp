#include "RangeTransition.h"
#include "Application2D.h"
#include "Transform.h"
#include "TradingAgent.h"

//checks if the transition's condition/s have been satisfied
bool RangeTransition::checkCondition()
{

	//cast the base agent to it's true type
	TradingAgent* tradingAgent = (TradingAgent*)(FSM->agent);

	//relative vector from this agent to the target
	Vector2 relative = (*tradingAgent->targetVector - tradingAgent->transform->translation);

	//if the target has been reached (within the desireable range)
	return relative.sqrMagnitude() < tradingAgent->targetRange * tradingAgent->targetRange;
}
