#include "TradingAgent.h"
#include "Application2D.h"
#include <iostream>

//destructor, deletes the contained FSM
TradingAgent::~TradingAgent()
{
}

//called once per frame, updates the FSM
void TradingAgent::update()
{
	std::cout << ((TradingAgent*)FSM->agent)->inventory.size() << '\n';

	//update the FSM
	FSM->update(appPtr->m_deltaTime);
}
