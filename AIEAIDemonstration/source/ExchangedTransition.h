#pragma once
#include "FiniteStateMachine.h"

/*
* class ExchangedTransition
* child class of Transition
*
* a state for the trading bot that becomes active
* when the agent has exchanged all items in the current trade
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class ExchangedTransition : public Transition
{

public:

	/*
	* ExchangedTransition()
	* default constructor
	*/
	ExchangedTransition() {}


	/*
	* ~ExchangedTransition()
	* default destructor
	*/
	~ExchangedTransition() {}


	/*
	* checkCondition
	* override's Transition's checkCondition()
	*
	* checks if the transitioning condition for
	* the transition has been met
	*
	* @returns bool - indicating if the condition was met
	*/
	bool checkCondition() override;

};