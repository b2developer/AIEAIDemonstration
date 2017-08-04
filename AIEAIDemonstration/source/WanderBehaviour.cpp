#include "WanderBehaviour.h"
#include "SteeringBehaviourManager.h"
#include "Transform.h"
#include "Application2D.h"

//called once per frame, gets the force to sum to the boid
Vector2 WanderBehaviour::update()
{
	//vector to add to the random vector
	Vector2 jitterVector = Vector2(rand() % 2000 - 1000.0f, rand() % 2000 - 1000.0f).normalised() * jitterAmount;

	//add the jitter vector to the random vector
	randomVector += jitterVector * sbm->appPtr->m_deltaTime;

	//clamp the random vector to the wander radius
	randomVector = randomVector.normalised() * wanderRadius;

	//the final vector, this will be seeked towards
	Vector2 wanderTarget = sbm->transform->translation + sbm->heading * wanderDistance + randomVector;

	//relative vector from the transform to the wander target
	Vector2 V = (wanderTarget - sbm->transform->translation).normalised() * sbm->maxVelocity;
	return V - sbm->entity->velocity;
}
