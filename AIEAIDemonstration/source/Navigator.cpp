#include "Navigator.h"
#include "Application2D.h"
#include "Transform.h"

//gets called once per frame, updates the path
void Navigator::update()
{
	//if there is a path to transverse
	if (path.size() > 0)
	{
		//next point to travel towards
		Vector2 next = path[0];
		Vector2 relative = next - transform->translation;

		float movementStepSqr = (movementSpeed * appPtr->m_deltaTime) * (movementSpeed * appPtr->m_deltaTime);

		//if the amount of units moved is smaller than the relative vector to the target
		if (movementStepSqr > relative.sqrMagnitude())
		{
			transform->translation = next;

			//erase the vector
			path.erase(path.begin());
		}
		else
		{
			//simply move towards the point
			transform->translation += relative.normalised() * movementSpeed * appPtr->m_deltaTime;
		}
	}
	else
	{
		int randomNumber = rand() % navMesh->data.vertices.size();

		setTarget(navMesh->data.vertices[randomNumber]->data->position);
	}
}

//requests a path to the closest node
void Navigator::setTarget(Vector2 target)
{
	Vertex<NavNode*, NavConnection*>* startVertex = nullptr;
	Vertex<NavNode*, NavConnection*>* endVertex = nullptr;

	//track the smallest distance
	float bestDistance = INFINITY;

	//iterate through all vertices in the NavMesh, searching for the closest vertex to the navigator
	for (size_t i = 0; i < navMesh->data.vertices.size(); i++)
	{
		//store in a temporary value for performance and readability
		Vertex<NavNode*, NavConnection*>* vertex = navMesh->data.vertices[i];
		float distance = (vertex->data->position - transform->translation).magnitude();

		//if the current distance is lower than the previously best distance
		if (distance < bestDistance)
		{
			//replace the best distance and vertex
			startVertex = vertex;
			bestDistance = distance;
		}

	}

	//reset the smallest distance
	bestDistance = INFINITY;

	//iterate through all vertices in the NavMesh, searching for the closest vertex to the navigator
	for (size_t i = 0; i < navMesh->data.vertices.size(); i++)
	{
		//store in a temporary value for performance and readability
		Vertex<NavNode*, NavConnection*>* vertex = navMesh->data.vertices[i];
		float distance = (vertex->data->position - target).magnitude();

		//if the current distance is lower than the previously best distance
		if (distance < bestDistance)
		{
			//replace the best distance and vertex
			endVertex = vertex;
			bestDistance = distance;
		}

	}

	//request a path from the navmesh
	path = navMesh->findRawPath(startVertex, endVertex);
	path = navMesh->smoothPath(path);

}
