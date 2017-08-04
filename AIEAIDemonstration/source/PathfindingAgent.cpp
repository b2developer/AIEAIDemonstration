#include "PathfindingAgent.h"
#include "Application2D.h"
#include "Transform.h"

//called once per frame
void PathfindingAgent::update()
{
	//early exit if the required navmesh doesn't exist
	if (navMesh == nullptr)
	{
		return;
	}

	if (pathList.size() > 0)
	{
		//follow the path if it still exists
		Vector2 point = pathList[0];

		//relative vector from the transform to the point
		Vector2 relative = point - transform->translation;

		//if the planned movement is further than the distance planned to be travelled
		if (relative.sqrMagnitude() <= (agentSpeed * agentSpeed * appPtr->m_deltaTime * appPtr->m_deltaTime))
		{
			transform->translation = point;

			//remove the first point in the list
			pathList.erase(pathList.begin());
		}
		else
		{
			transform->translation += relative.normalised() * agentSpeed * appPtr->m_deltaTime;
		}
	}
	else
	{
		//there is no path, find a new end-point and get a new path
		size_t randNum = rand() % navMesh->data.vertices.size();

		//calculate a new path
		pathList = navMesh->findPath(transform->translation, navMesh->data.vertices[randNum]->data->position, radius);

		//this means that the end point isn't accessable due to shoulder width
		if (pathList.size() == 0)
		{
			//there is no path, find a new end-point and get a new path
			randNum = rand() % navMesh->data.vertices.size();

			//calculate a new path
			pathList = navMesh->findPath(transform->translation, transform->translation, radius);

			//this means that the agent isn't on the NavMesh yet
			if (pathList.size() == 0)
			{
				//there is no path, find a new end-point and get a new path
				randNum = rand() % navMesh->data.vertices.size();
				size_t randNum2 = rand() % navMesh->data.vertices.size();

				//calculate a new path
				pathList = navMesh->findPath(navMesh->data.vertices[randNum]->data->position, navMesh->data.vertices[randNum2]->data->position, radius);
			}
		}
	}
}
