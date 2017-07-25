#include "ExchangedTransition.h"
#include "Application2D.h"
#include "Transform.h"
#include "TradingAgent.h"

//checks if the transition's condition/s have been satisfied
bool ExchangedTransition::checkCondition()
{
	//cast the base agent to it's true type
	TradingAgent* tradingAgent = (TradingAgent*)(FSM->agent);

	return tradingAgent->trade == nullptr
		|| ((BlackboardData<Trade, Trade>*)tradingAgent->trade->item)->data.give.size() == 0
		&& ((BlackboardData<Trade, Trade>*)tradingAgent->trade->item)->data.recieve.size() == 0;
}