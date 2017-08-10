#pragma once
#include "Agent.h"

//type for reponses from a node
enum class BehaviourResponse
{
	SUCCESS, //returned when the node succeeds in it's condition/task
	FAILURE, //returned when the node fails in it's condition/task
	PENDING, //returned when the node is yet to determine the correct response
};


//forward declaration for Behaviour
class BehaviourTree;

/*
* class Behaviour
*
* base class for a node of a behaviour tree, updated when neccessary.
* evaluates to a SUCCESS, FAILURE or PENDING depending on the
* outcome of the behaviour
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Behaviour
{

public:

	//pointer to the tree that holds this behaviour
	BehaviourTree* tree = nullptr;

	/*
	* Behaviour()
	* default contstructor
	*/
	Behaviour() {};


	/*
	* ~Behaviour()
	* virtual function
	* default destructor
	*/
	virtual ~Behaviour() {};


	/*
	* update
	* abstract function
	*
	* called by the behaviour tree when required
	* evaluates to either SUCCESS, FAILURE or PENDING
	* the evalutation of a node may cause other nodes
	* to not be activated depending on the structure
	*
	* @param float deltaTime - the amount of time passed since the last frame
	* @returns BehaviourResponse - the response from the behaviour
	*/
	virtual BehaviourResponse update(float deltaTime) = 0;

};



/*
* class BehaviourTree
*
* a tree stucture of behaviours that describe completely how an AI
* behaves in small trivial tasks and determining this with pseudo
* boolean expressions
* 
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class BehaviourTree
{

public:

	//the agent that holds this tree
	Agent* agent = nullptr;

	//the root behaviour of the tree
	Behaviour* root = nullptr;

	/*
	* BehaviourTree()
	* default contstructor
	*/
	BehaviourTree() {};


	/*
	* ~BehaviourTree()
	* destructor, causes the nodes to delete themselves
	*/
	~BehaviourTree();


	/*
	* update
	*
	* updates the root node, which in the right circumstances
	* updates other nodes recursively
	*
	* @param float deltaTime - the amount of time passed since the last frame
	* @returns void
	*/
	void update(float deltaTime);

};