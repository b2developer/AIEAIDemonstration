#include "FleeBehaviour.h"
#include "SteeringBehaviourManager.h"
#include "Transform.h"
#include "Application2D.h"

//called once per frame, gets the force to sum to the boid
Vector2 FleeBehaviour::update()
{
	//relative vector from the target to the transform
	Vector2 V = (sbm->transform->translation - *target).normalised() * sbm->maxVelocity;
	return V - sbm->entity->velocity;
}
