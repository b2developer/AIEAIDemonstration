#pragma once
#include "FiniteStateMachine.h"
#include "TradingAgent.h"


//how long trades take to expire on the blackboard
const float TRADE_LIFETIME = 5.0f;

/*
* class SearchState
* child class of State
*
* a state for the trading bot makes the bot search
* for and post to trades on a blackboard
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class SearchState : public State
{

public:

	/*
	* SearchState()
	* default constructor
	*/
	SearchState() {}


	/*
	* ~SearchState()
	* default destructor
	*/
	~SearchState() {}


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


	/*
	* postRandomTrade
	*
	* makes the trading agent post a random trade
	* onto the blackboard by selecting random
	* items from it's inventory
	*
	* @param unsigned int itemsLost - the amount of items that the agent is going to trade
	* @param unsigned int itemsGained - the amount of items that the agent is going to recieve back
	* @returns void
	*/
	void postRandomTrade(unsigned int itemsLost, unsigned int itemsGained);

	
	/*
	* compatibleTrade
	*
	* analyses a trade and determines
	* if the agent has the necessary items
	* to complete it
	*
	* @param Trade* trade - pointer to the trade being analysed
	* @returns void
	*/
	bool compatibleTrade(Trade* trade);

};