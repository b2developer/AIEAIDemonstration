#include "ArrivalBehaviour.h"
#include "SteeringBehaviourManager.h"
#include "Transform.h"
#include "Application2D.h"

//called once per frame, gets the force to sum to the boid
Vector2 ArrivalBehaviour::update()
{
	//relative vector from the transform to the target
	Vector2 V = *target - sbm->transform->translation;

	//scale down velocity based on the distance and the arrival radius
	float distanceToTarget = V.magnitude();
	float scale = fminf(distanceToTarget / arrivalRadius, sbm->maxVelocity);

	return V.normalised() * scale - sbm->entity->velocity;
}
