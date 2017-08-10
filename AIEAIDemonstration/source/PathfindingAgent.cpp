#include "PathfindingAgent.h"
#include "Application2D.h"
#include "Transform.h"
#include "BehaviourTree.h"

//destructor, deletes the behaviour tree
PathfindingAgent::~PathfindingAgent()
{
	delete behaviourTree;
}

//called once per frame
void PathfindingAgent::update()
{
	//early exit if the required navmesh or behaviour tree doesn't exist
	if (navMesh == nullptr || behaviourTree == nullptr)
	{
		return;
	}
	
	//update the behaviour tree
	behaviourTree->update(appPtr->m_deltaTime);
}
