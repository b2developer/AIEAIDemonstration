#include "PathfindingBotSpawner.h"
#include "Application2D.h"

#include "Transform.h"
#include "Box.h"
#include "Circle.h"
#include "Renderer.h"
#include "PathfindingAgent.h"
#include "BehaviourTrees.h"

#include <iostream>
#include <stdio.h>

/*
* havePathFunction
*
* tests if the agent has found
* a path to follow to a destination
*
* @param Agent* agent - the agent that called this function (actually called by a behaviour in the agent's tree)
* @returns BehaviourResponse - indicates if the agent has a path or not
*/
BehaviourResponse havePathFunction(Agent* agent)
{
	//up-cast the base Agent pointer
	PathfindingAgent* pathAgent = (PathfindingAgent*)agent;

	//a path exists if pathList contains any more points
	if (pathAgent->pathList.size() > 0)
	{
		return BehaviourResponse::SUCCESS;
	}
	else
	{
		return BehaviourResponse::FAILURE;
	}
}



/*
* followPathFunction
*
* an action that follows a previously
* calculated path while there are still points
* to follow
*
* @param Agent* agent - the agent that called this function (actually called by a behaviour in the agent's tree)
* @returns BehaviourResponse - always SUCCESS since this is an action
*/
BehaviourResponse followPathFunction(Agent* agent)
{
	//up-cast the base Agent pointer
	PathfindingAgent* pathAgent = (PathfindingAgent*)agent;

	//next point to travel to
	Vector2 nextPoint = pathAgent->pathList[0];

	//relative vector to the next point
	Vector2 relative = nextPoint - pathAgent->transform->translation;
	
	//check if the planned movement is further than the distance
	if (relative.sqrMagnitude() < pathAgent->agentSpeed * pathAgent->agentSpeed * pathAgent->appPtr->m_deltaTime * pathAgent->appPtr->m_deltaTime)
	{
		//snap to the next point, remove it from the list
		agent->transform->translation = nextPoint;

		pathAgent->pathList.erase(pathAgent->pathList.begin());
	}
	else
	{
		//move towards the next point
		agent->transform->translation += relative.normalised() * pathAgent->agentSpeed * pathAgent->appPtr->m_deltaTime;
	}

	return BehaviourResponse::SUCCESS;
}



/*
* getPathFunction
*
* an action that calls upon the navmesh
* to calculate a new path to follow, can
* return PENDING if the agent calculated a path
* too recently
*
* @param Agent* agent - the agent that called this function (actually called by a behaviour in the agent's tree)
* @returns BehaviourResponse - always SUCCESS since this is an action
*/
BehaviourResponse getPathFunction(Agent* agent)
{
	//up-cast the base Agent pointer
	PathfindingAgent* pathAgent = (PathfindingAgent*)agent;

	pathAgent->generationTimer -= agent->appPtr->m_deltaTime;

	//if the timer has counted down
	if (pathAgent->generationTimer <= 0.0f)
	{
		//reset the timer
		pathAgent->generationTimer = pathAgent->generationDuration;
	}
	else
	{
		return BehaviourResponse::PENDING;
	}

	//randomly select two random points
	size_t r1 = rand() % pathAgent->navMesh->data.vertices.size();
	size_t r2 = rand() % pathAgent->navMesh->data.vertices.size();

	//attempt to find a path
	pathAgent->pathList = pathAgent->navMesh->findPath(pathAgent->transform->translation, pathAgent->navMesh->data.vertices[r1]->data->position, pathAgent->radius);

	//if a path was not found
	if (pathAgent->pathList.size() == 0)
	{
		//check if the agent is on a triangle it can fit on
		pathAgent->pathList = pathAgent->navMesh->findPath(pathAgent->transform->translation, pathAgent->transform->translation, pathAgent->radius);

		//the agent is on a triangle it can fit on
		if (pathAgent->pathList.size() > 0)
		{
			return BehaviourResponse::FAILURE;
		}

		pathAgent->pathList = pathAgent->navMesh->findPath(pathAgent->navMesh->data.vertices[r1]->data->position, pathAgent->navMesh->data.vertices[r2]->data->position, pathAgent->radius);

		if (pathAgent->pathList.size() == 0)
		{
			return BehaviourResponse::SUCCESS;
		}
		else
		{
			return BehaviourResponse::FAILURE;
		}
	}

	return BehaviourResponse::SUCCESS;
}



/*
* findingPathPreFunction
*
* a decorator pre-process function, does nothing
*
* @param BehaviourDecorator* self - the behaviour decorator that called this
* @param Agent* agent - the agent that called this function (actually called by a behaviour in the agent's tree)
* @returns void
*/
void findingPathPreFunction(BehaviourDecorator* self, Agent* agent)
{
	//up-cast the base Agent pointer
	PathfindingAgent* pathAgent = (PathfindingAgent*)agent;
}



/*
* findingPathPostFunction
*
* a decorator post-process function, logs that a path was found
*
* @param BehaviourDecorator* self - the behaviour decorator that called this
* @param Agent* agent - the agent that called this function (actually called by a behaviour in the agent's tree)
* @param BehaviourResponse response - the final response to return to the parent
* @returns BehaviourResponse - always SUCCESS since this is an action
*/
BehaviourResponse findingPathPostFunction(BehaviourDecorator* self, Agent* agent, BehaviourResponse response)
{
	//if the response is a SUCCESS, announce it
	if (response == BehaviourResponse::SUCCESS)
	{
		std::cout << "A path was successfully found.\n";
	}

	return response;
}




//adds the necessary components for a pathfinding bot object
void PathfindingBotSpawner::addComponents(GameObject * creation)
{
	//add all components
	Transform* transform = new Transform();
	Circle* circle = new Circle();
	Box* box = new Box();
	Renderer* renderer = new Renderer();
	Agent* pathfindingAgent = new PathfindingAgent();

	transform->scale = Vector2(1.0f, 1.0f);
	transform->rotation = 0.0f;

	//give the circle a random radius
	circle->radius = ((rand() % 700) + 300) * (30.0f / 1000.0f);

	//give the NavMesh and shoulder radius to the agent
	((PathfindingAgent*)pathfindingAgent)->navMesh = navMesh;
	((PathfindingAgent*)pathfindingAgent)->radius = circle->radius;

	//create the behaviour tree, nodes and function pointers for the tree to use
	BehaviourTree* behaviourTree = new BehaviourTree();

	behaviourTree->agent = pathfindingAgent;

	BehaviourSelector* root = new BehaviourSelector();
	root->tree = behaviourTree;

	behaviourTree->root = root;

	BehaviourSequence* checkPath = new BehaviourSequence();
	checkPath->tree = behaviourTree;

	BehaviourCondition* havePath = new BehaviourCondition();
	havePath->tree = behaviourTree;

	BehaviourAction* followPath = new BehaviourAction();
	followPath->tree = behaviourTree;

	BehaviourDecorator* logPath = new BehaviourDecorator();
	logPath->tree = behaviourTree;

	BehaviourAction* getPath = new BehaviourAction();
	getPath->tree = behaviourTree;

	//linking nodes to parents
	root->children.push_back(checkPath);
	root->children.push_back(logPath);

	checkPath->children.push_back(havePath);
	checkPath->children.push_back(followPath);

	logPath->child = getPath;
	
	//actual function pointers
	havePath->evaluation = &havePathFunction;
	followPath->execution = &followPathFunction;
	getPath->execution = &getPathFunction;
	logPath->preProcess = &findingPathPreFunction;
	logPath->postProcess = &findingPathPostFunction;

	//give the agent it's behaviour tree
	((PathfindingAgent*)pathfindingAgent)->behaviourTree = behaviourTree;

	//construct the box to match the circle
	box->min = Vector2(-circle->radius, -circle->radius);
	box->max = Vector2(circle->radius, circle->radius);

	char texturePath[FILENAME_MAX];

	//load the navigation bot texture for the renderer
	strcpy_s(texturePath, appPtr->resourceFolder);
	strcat_s(texturePath, "textures\\navBot.png");
	renderer->load(texturePath);

	//linking the components together
	renderer->box = box;
	renderer->appPtr = appPtr;
	circle->transform = transform;
	box->transform = transform;
	pathfindingAgent->appPtr = appPtr;
	pathfindingAgent->transform = transform;

	//link the components to the game object
	transform->container = creation;
	circle->container = creation;
	box->container = creation;
	renderer->container = creation;
	pathfindingAgent->container = creation;

	//add the components to the list
	creation->components.push_back(transform);
	creation->components.push_back(circle);
	creation->components.push_back(box);
	creation->components.push_back(renderer);
	creation->components.push_back(pathfindingAgent);

}
