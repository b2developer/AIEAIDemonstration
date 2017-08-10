#include "TradingAgent.h"
#include "Application2D.h"

//destructor, deletes the contained FSM and all proposed trade references
TradingAgent::~TradingAgent()
{
	//iterate through all proposed trades, deleting each
	for (size_t i = 0; i < tradeRefs.size(); i++)
	{
		//store in a temporary value for performance and readability
		BlackboardReference* ref = tradeRefs[i];

		delete ref;
	}

	delete FSM;
}

//called once per frame, updates the FSM
void TradingAgent::update()
{
	//update the FSM
	FSM->update(appPtr->m_deltaTime);
}
