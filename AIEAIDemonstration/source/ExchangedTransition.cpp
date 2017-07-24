#include "ExchangedTransition.h"
#include "Application2D.h"
#include "Transform.h"
#include "TradingAgent.h"

//checks if the transition's condition/s have been satisfied
bool ExchangedTransition::checkCondition()
{

	//cast the base agent to it's true type
	TradingAgent* tradingAgent = (TradingAgent*)(FSM->agent);

	//have both lists of items to give been depleted
	return tradingAgent->currentData == nullptr || (tradingAgent->currentData->data.give.size() == 0 && tradingAgent->currentData->data.recieve.size() == 0) || tradingAgent->currentData->timeLocked == false;
}