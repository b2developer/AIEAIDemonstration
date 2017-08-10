#pragma once
#include "Agent.h"
#include <vector>
#include "MathsLibrary.h"
#include "NavMesh.h"

//forward declarations
class Transform;
class Application2D;
class BehaviourTree;


/*
* class PathfindingAgent
* child class of Agent
*
* a component that navigates through a navmesh
* by choosing random points on it
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class PathfindingAgent : public Agent
{
public:

	//pointer to the tree that controls the agent
	BehaviourTree* behaviourTree = nullptr;

	//pointer to the mesh to navigate around
	NavMesh* navMesh = nullptr;

	//list of vectors that make up the path to travel
	std::vector<Vector2> pathList;

	//the half shoulder-width of the agent, used to find parts of the NavMesh that can be travelled through
	float radius = 0.0f;

	//units that the agent can move per second
	float agentSpeed = 100.0f;

	//timer to track how long its been since the last path was created
	float generationTimer = 0.0f;

	//the set duration to reset the generation timer to
	float generationDuration = 5.0f;

	/*
	* PathfindingAgent()
	* default constructor
	*/
	PathfindingAgent() {}


	/*
	* ~PathfindingAgent()
	* destructor, deletes the behaviour tree
	*/
	~PathfindingAgent();


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