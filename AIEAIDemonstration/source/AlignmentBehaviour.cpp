#include "AlignmentBehaviour.h"
#include "SteeringBehaviourManager.h"
#include "Transform.h"
#include "Application2D.h"

//called once per frame, gets the force to sum to the boid
Vector2 AlignmentBehaviour::update()
{
	//container for neighbouring boids
	std::vector<SteeringBehaviourManager*> neighbours;

	//iterate through all other boids, remembering the boids which are not within the radius
	for (size_t i = 0; i < sbm->otherBoids.size(); i++)
	{
		//store in a temporary value for performance and readability
		SteeringBehaviourManager* otherBoid = sbm->otherBoids[i].sbm;

		//this object can't be it's own neighbour
		if (otherBoid == sbm)
		{
			continue;
		}

		//get the distance to the other boid
		float sqrMagnitude = (otherBoid->transform->translation - (sbm->transform->translation + sbm->heading * neighbourhoodForwardDistance)).sqrMagnitude();

		//if the boid is inside the radius
		if (sqrMagnitude < neighbourhoodRadius * neighbourhoodRadius)
		{
			neighbours.push_back(otherBoid);
		}
	}

	//average velocity of all boids
	Vector2 average = Vector2(0, 0);

	//iterate through all neighbours
	for (size_t i = 0; i < neighbours.size(); i++)
	{
		//store in a temporary value for performance and readability
		SteeringBehaviourManager* otherBoid = neighbours[i];

		average += otherBoid->entity->velocity;
	}

	//don't divide by 0
	if (neighbours.size() > 0)
	{
		average = average / (float)neighbours.size();
	}

	return average - sbm->entity->velocity;
}
