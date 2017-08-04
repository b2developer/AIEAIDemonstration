#include "EvadeBehaviour.h"
#include "SteeringBehaviourManager.h"
#include "Transform.h"
#include "Application2D.h"

//called once per frame, gets the force to sum to the boid
Vector2 EvadeBehaviour::update()
{
	//relative vector from the predicted future position of the target boid to the transform
	Vector2 V = sbm->transform->translation - targetBoid->transform->translation + targetBoid->entity->velocity;
	return V.normalised() * sbm->maxVelocity - sbm->entity->velocity;
}
