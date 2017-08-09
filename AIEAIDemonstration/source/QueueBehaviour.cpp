#include "QueueBehaviour.h"
#include "SteeringBehaviourManager.h"
#include "Transform.h"
#include "Application2D.h"

//called once per frame, gets the force to sum to the boid
Vector2 QueueBehaviour::update()
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

		//if the boid is outside the radius
		if (sqrMagnitude < neighbourhoodRadius * neighbourhoodRadius)
		{
			neighbours.push_back(otherBoid);
		}
	}

	Vector2 closestBoid = Vector2(0, 0);
	float closestSqrMag = INFINITY;

	//iterate through all neighbours
	for (size_t i = 0; i < neighbours.size(); i++)
	{
		//store in a temporary value for performance and readability
		SteeringBehaviourManager* otherBoid = neighbours[i];

		//position of the following point
		Vector2 followPoint = otherBoid->transform->translation + otherBoid->heading * -queueDistance;

		Vector2 otherBoidPos = otherBoid->transform->translation - sbm->transform->translation;

		//only follow the boid if it is in front of this one
		if (sbm->heading.dot(otherBoidPos) > 0)
		{
			float sqrMag = otherBoidPos.sqrMagnitude();

			if (closestSqrMag > sqrMag)
			{
				closestBoid = followPoint;
				closestSqrMag = sqrMag;
			}
		}
	}

	//a suitable neighbouring boid wasn't found
	if (closestSqrMag == INFINITY)
	{
		return Vector2(0, 0);
	}

	return (closestBoid - sbm->transform->translation).normalised() * sbm->maxVelocity - sbm->entity->velocity;
}