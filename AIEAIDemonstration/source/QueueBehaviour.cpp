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

	//average neighbour hood vector to all boids to be attracted to
	Vector2 average = Vector2(0, 0);

	//iterate through all neighbours
	for (size_t i = 0; i < neighbours.size(); i++)
	{
		//store in a temporary value for performance and readability
		SteeringBehaviourManager* otherBoid = neighbours[i];

		//relative vector to the trailing point of the other boid
		Vector2 relative = otherBoid->transform->translation + otherBoid->heading * -queueDistance;

		Vector2 otherBoidPos = otherBoid->transform->translation - sbm->transform->translation;

		//only follow the boid if it is in front of this one
		if (sbm->heading.dot(otherBoidPos) > 0)
		{
			average += relative;
		}
	}

	//don't divide by 0
	if (neighbours.size() > 0)
	{
		average = average / (float)neighbours.size();
	}
	else
	{
		return Vector2(0, 0);
	}


	return (average - sbm->transform->translation).normalised() * sbm->maxVelocity - sbm->entity->velocity;
}