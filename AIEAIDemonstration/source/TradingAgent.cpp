#include "TradingAgent.h"
#include "Application2D.h"

//destructor, deletes the contained FSM
TradingAgent::~TradingAgent()
{
	delete FSM;
}

//called once per frame, updates the FSM
void TradingAgent::update()
{
	//update the FSM
	FSM->update(appPtr->m_deltaTime);
}
