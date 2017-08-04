#include "PursueBehaviour.h"
#include "SteeringBehaviourManager.h"
#include "Transform.h"
#include "Application2D.h"

//called once per frame, gets the force to sum to the boid
Vector2 PursueBehaviour::update()
{
	//relative vector from the transform to the predicted future position of the target boid
	Vector2 V = targetBoid->transform->translation + targetBoid->entity->velocity - sbm->transform->translation;
	return V.normalised() * sbm->maxVelocity - sbm->entity->velocity;
}
