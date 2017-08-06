#include "SeparationBehaviour.h"
#include "SteeringBehaviourManager.h"
#include "Transform.h"
#include "Application2D.h"

//called once per frame, gets the force to sum to the boid
Vector2 SeparationBehaviour::update()
{
	//container for neighbouring boids
	std::vector<SteeringBehaviourManager*> neighbours;

	//iterate through all other boids, remembering the boids which are within the radius
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
		float sqrMagnitude = (otherBoid->transform->translation - sbm->transform->translation).sqrMagnitude();

		//if the boid is outside the radius
		if (sqrMagnitude < neighbourhoodRadius * neighbourhoodRadius)
		{
			neighbours.push_back(otherBoid);
		}
	}

	//sum the relative vector to all boids to be seperated from
	Vector2 repel = Vector2(0, 0);

	//iterate through all neighbours
	for (size_t i = 0; i < neighbours.size(); i++)
	{
		//store in a temporary value for performance and readability
		SteeringBehaviourManager* otherBoid = neighbours[i];

		//relative vector to the other boid
		Vector2 relative = sbm->transform->translation - otherBoid->transform->translation;

		float magnitude = relative.magnitude() / neighbourhoodRadius;

		//clamp the division
		if (magnitude < 0.01f)
		{
			magnitude = 0.01f;
		}

		repel += relative / magnitude;
	}

	return repel.normalised() * sbm->maxVelocity - sbm->entity->velocity;
}
