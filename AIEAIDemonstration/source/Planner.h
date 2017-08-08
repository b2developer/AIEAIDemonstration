#pragma once

#include <vector>

#define TEMPLATE template<typename T>
#define TEMPLATE2 template<typename T, typename U>

//forward declarations
class PlannerAction;
class PlannerState;

#pragma region Planner
/*
* class Planner
*
* an object that searches through a domain of states
* by applying actions to a starting state until a goal state is found
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Planner
{

public:

	//pointer to the state to navigate towards
	PlannerState* goalState = nullptr;

	/*
	* Planner()
	* default constructor
	*/
	Planner() {}


	/*
	* ~Planner()
	* destructor, deletes the goal state
	*/
	~Planner();


	/*
	* solveState
	*
	* uses A* to determine a set of actions
	* to manipulate the given state into the goal state
	*
	* @param PlannerState* original - the current state to gets actions for
	* @returns std::vector<PlannerAction*> - a list of actions that when applied turns the original state into the goal state
	*/
	std::vector<PlannerAction*> solveState(PlannerState* original);

};
#pragma endregion



#pragma region PlannerState
/*
* class PlannerState
*
* a state that can be formed by the planner, applying an action 
* to a state generates a new state, states also know which 
* actions can be applied to them
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class PlannerState
{

public:

	//pointer to the action that formed this state
	PlannerAction* previousAction = nullptr;

	//pointer to the previous state
	PlannerState* previous = nullptr;

	//cost to get to this state using actions
	float gScore = INFINITY;

	//heuristic score of the state
	float hScore = 0.0f;

	/*
	* PlannerState()
	* default constructor
	*/
	PlannerState() {}


	/*
	* ~PlannerState()
	* virtual function
	* default destructor
	*/
	virtual ~PlannerState() {}

	
	/*
	* setHeuristic
	* abstract function
	*
	* gets the 'H' score of the state by
	* comparing itself to the goal state
	*
	* @param PlannerState* goal - the goal state to compare to
	* @returns void
	*/
	virtual void setHeuristic(PlannerState* goal) = 0;


	/*
	* getActions
	* abstract function
	* 
	* gets a list of actions that can be applied 
	* to the state and generate a different state
	*
	* @returns std::vector<PlannerAction*> - the list of actions that can be applied to this state
	*/
	virtual std::vector<PlannerAction*> getActions() = 0;

};
#pragma endregion



#pragma region PlannerAction
/*
* class PlannerAction
*
* base class for an operation that can be applied
* to a state to generate a new state
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class PlannerAction
{

public:

	//the cost of applying this action to a state
	float cost = 1.0f;

	//indicates if the action needs to be kept and not deleted by the planner after use
	bool keep = false;

	/*
	* PlannerAction()
	* default constructor
	*/
	PlannerAction() {}


	/*
	* ~PlannerAction()
	* virtual function
	* default destructor
	*/
	virtual ~PlannerAction() {};


	/*
	* execute
	* abstract function
	*
	* applies the action to the state
	* and returns it as a new state
	*
	* @param PlannerState* original - the original state to apply the action to
	* @returns PlannerState* - the new state with the action applied to it
	*/
	virtual PlannerState* execute(PlannerState* original) = 0;


	/*
	* isReverse
	* abstract function
	*
	* checks if this action applies the opposite
	* effect of the other action
	*
	* @param PlannerAction* other - pointer to the other action
	* @returns bool - indicates if 'other' is the reverse action
	*/
	virtual bool isReverse(PlannerAction* other) = 0;

};
#pragma endregion