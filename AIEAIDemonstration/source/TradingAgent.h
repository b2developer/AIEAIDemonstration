#pragma once
#include "Agent.h"
#include <vector>
#include "FiniteStateMachine.h"
#include "Blackboard.h"
#include "MathsLibrary.h"

//forward declaration
class TradingAgent;


const int OBJECT_SIZE = 4;

//type for objects that an agent can hold
enum ObjectType
{
	APPLE = 0,
	BANANA = 1,
	CHERRY = 2,
	ORANGE = 3,
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

	//the current trade being performed
	BlackboardReference* trade = nullptr;

	//the trading agent to trade with
	TradingAgent* other = nullptr;

	//did this agent propose the trade?
	bool initiator = false;



	//SEARCH STATE VARIABLES
	//----------------------------------------------------------
	//maximum amount of trades the agent can propose at one time
	int maxTrades = 3;

	//the amount of items given/lost during trades
	int itemsLostDuringTrades = 4;
	int itemsGainedDuringTrades = 4;

	//amount of seconds the trade request lasts
	float tradeExpiryTime = 5.0f;

	//container for trades this agent proposed to the blackboard
	std::vector<BlackboardReference*> tradeRefs;
	//----------------------------------------------------------



	//GOTO STATE / RANGE TRANSITION VARIABLES
	//----------------------------------------------------------
	//how close two trading agents must be to trade
	float targetRange = 120.0f;

	//units moved per second
	float movementSpeed = 100.0f;

	Vector2 origin = Vector2(0, 0);

	//target vector (an agent or an origin)
	Vector2* targetVector = nullptr;
	//----------------------------------------------------------



	//EXCHANGE STATE / EXCHANGED TRANSITION VARIABLES
	//----------------------------------------------------------
	//timer for the exchanges
	float exchangeTimer = 0.0f;

	//how long it takes to give one item to the other agent
	float exchangeDuration = 1.0f;
	//----------------------------------------------------------


	/*
	* TradingAgent()
	* default constructor
	*/
	TradingAgent() {}


	/*
	* ~TradingAgent()
	* destructor, deletes the contained FSM and proposed trade
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