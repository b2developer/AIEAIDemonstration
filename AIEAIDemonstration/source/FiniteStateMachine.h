#pragma once

#include "Graph.h"
#include "Agent.h"

//forward declaration
class FiniteStateMachine;

#pragma region Transition
/*
* class Transition
*
* base object for a transition, a class that describes
* a flow path from one state to another
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Transition
{

public:

	//link to the holder of this transition
	FiniteStateMachine* FSM = nullptr;

	/*
	* Transition()
	* default constructor
	*/
	Transition() {}


	/*
	* ~Transition()
	* virtual function
	* default destructor
	*/
	virtual ~Transition() {}


	/*
	* checkCondition
	* abstract function
	*
	* checks if the transitioning condition for
	* the transition has been met
	*
	* @returns bool - indicating if the condition was met
	*/
	virtual bool checkCondition() = 0;

};
#pragma endregion



#pragma region State
/*
* class State
*
* base object for a state, a class that describes behaviour
* that an entity can have (eg. happy, angry, sad)
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class State
{

public:

	//link to the holder of this state
	FiniteStateMachine* FSM = nullptr;

	/*
	* State()
	* default constructor
	*/
	State() {}


	/*
	* ~State()
	* virtual function
	* default destructor
	*/
	virtual ~State() {}


	/*
	* update
	* abstract function
	*
	* gets called once per frame
	* while the state is active within the
	* finite state machine
	*
	* @param float deltaTime - the amount of time passed since the last frame
	* @returns void
	*/
	virtual void update(float deltaTime) = 0;


	/*
	* onEnter
	* abstract function
	*
	* gets called when the finite state machine
	* transitions into this state
	*
	* @param float deltaTime - the amount of time passed since the last frame
	* @returns void
	*/
	virtual void onEnter(float deltaTime) = 0;


	/*
	* onExit
	* abstract function
	*
	* gets called when the finite state machine
	* transitions out of this state
	*
	* @param float deltaTime - the amount of time passed since the last frame
	* @returns void
	*/
	virtual void onExit(float deltaTime) = 0;

};
#pragma endregion



#pragma region FSM
/*
* class FiniteStateMachine
*
* a list of states contained in a graph, every vertex is a state
* every edge is a transition
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class FiniteStateMachine
{

public:
	
	Agent* agent = nullptr; //pointer to the agent who holds this state machine
	Vertex<State*, Transition*>* currentVertex = nullptr; //pointer to the current state's vertex
	Graph<State*, Transition*> data; //graph structure that contains the states and transition

	/*
	* FiniteStateMachine()
	* default constructor
	*/
	FiniteStateMachine() {}


	/*
	* ~FiniteStateMachine()
	* default destructor
	*/
	~FiniteStateMachine() {}


	/*
	* update
	*
	* gets called once per frame
	* while the state machine itself
	* is active
	*
	* @param float deltaTime - the amount of time passed since the last frame
	* @returns void
	*/
	void update(float deltaTime);

	
	/*
	* setLinks
	*
	* links all of the states, transitions and conditions
	* contained back to their container
	*
	* @returns void
	*/
	void setLinks();

};
#pragma endregion