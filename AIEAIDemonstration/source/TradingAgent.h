#pragma once
#include "Agent.h"
#include <vector>
#include "FiniteStateMachine.h"
#include "Blackboard.h"

//forward declaration
class TradingAgent;


const int OBJECT_SIZE = 4;

//type for objects that an agent can hold
enum ObjectType
{
	APPLE = 0,
	BANANA = 1,
	CHERRY = 2,
	GRAPE = 3,
};


/*
* struct Trade
*
* data structure for trading agents to observe trades with
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
struct Trade
{
	std::vector<ObjectType> recieve; //what the poster gives
	std::vector<ObjectType> give; //what the observer will give the poster

	TradingAgent* poster = nullptr; //the trading agent that proposed this trade
	TradingAgent* responder = nullptr; //the trading agent that responded to this trade
};


//forward declarations
class Transform;
class Application2D;

/*
* class TradingAgent
* child class of Agent
*
* a component that proposes trades and trades items
* with other gameobject that contain this component
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class TradingAgent : public Agent
{
public:

	FiniteStateMachine* FSM = nullptr; //the finite state machine that contains all of the logic
	Blackboard* tradingBlackboard = nullptr; //the blackboard that the trading agent uses

	//container for the objects that this trading agent has to offer
	std::vector<ObjectType> inventory;

	//container for trades this agent proposed to the blackboard
	std::vector<Trade*> proposedTrades;

	//container for the corresponding numbers that the blackboard assigned to the trade
	std::vector<int> proposedIds;

	/*
	* TradingAgent()
	* default constructor
	*/
	TradingAgent() {}


	/*
	* ~TradingAgent()
	* destructor, deletes the contained FSM
	*/
	~TradingAgent();


	/*
	* update
	* abstract function
	* overrides Agent's update()
	*
	* gets called once per frame, updates the state
	* of the trading agent
	*
	* @returns void
	*/
	void update() override;

};