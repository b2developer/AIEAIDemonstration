#include "SeekBehaviour.h"
#include "SteeringBehaviourManager.h"
#include "Transform.h"
#include "Application2D.h"

//called once per frame, gets the force to sum to the boid
Vector2 SeekBehaviour::update()
{
	//relative vector from the transform to the target
	Vector2 V = (*target - sbm->transform->translation).normalised() * sbm->maxVelocity;
	return V - sbm->entity->velocity;
}
