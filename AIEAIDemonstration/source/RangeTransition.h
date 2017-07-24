#pragma once
#include "FiniteStateMachine.h"

/*
* class RangeTransition
* child class of Transition
*
* a state for the trading bot that becomes active
* when the agent is in range of the target it has been travelling to
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class RangeTransition : public Transition
{

public:

	/*
	* RangeTransition()
	* default constructor
	*/
	RangeTransition() {}


	/*
	* ~RangeTransition()
	* default destructor
	*/
	~RangeTransition() {}


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