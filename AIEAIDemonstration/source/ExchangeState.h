#pragma once
#include "FiniteStateMachine.h"
#include "TradingAgent.h"


/*
* class ExchangeState
* child class of State
*
* a state for the trading bot that makes the bot
* give all of it's items that it agreed to trade
* to the other agent
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class ExchangeState : public State
{

public:

	/*
	* ExchangeState()
	* default constructor
	*/
	ExchangeState() {}


	/*
	* ~ExchangeState()
	* default destructor
	*/
	~ExchangeState() {}


	/*
	* update
	* overrides State's update(float deltaTime)
	*
	* gets called once per frame
	* while the state is active within the
	* finite state machine
	*
	* @param float deltaTime - the amount of time passed since the last frame
	* @returns void
	*/
	void update(float deltaTime) override;


	/*
	* onEnter
	* overrides State's onEnter(float deltaTime)
	*
	* gets called when the finite state machine
	* transitions into this state
	*
	* @param float deltaTime - the amount of time passed since the last frame
	* @returns void
	*/
	void onEnter(float deltaTime) override;


	/*
	* onExit
	* overrides State's onExit(float deltaTime)
	*
	* gets called when the finite state machine
	* transitions out of this state
	*
	* @param float deltaTime - the amount of time passed since the last frame
	* @returns void
	*/
	void onExit(float deltaTime) override;

};