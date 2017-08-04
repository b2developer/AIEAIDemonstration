#include "SteeringBehaviourManager.h"
#include "Transform.h"
#include "Application2D.h"
#include "SteeringBehaviour.h"

//destructor, removes all behaviours
SteeringBehaviourManager::~SteeringBehaviourManager()
{
	//iterate through all behaviours, deleting each one
	for (size_t i = 0; i < behaviours.size(); i++)
	{
		delete behaviours[i];
	}
}

//called once per frame, applies all forces as a combined (averaged) vector
void SteeringBehaviourManager::update()
{
	Vector2 forceSum = Vector2(0, 0);

	//iterate through all behaviours, calculating the force to add the sum
	for (size_t i = 0; i < behaviours.size(); i++)
	{
		//store in a temporary for performance and readabiltiy
		SteeringBehaviour* behaviour = behaviours[i];

		//get the force to add to the force sum
		Vector2 force = behaviour->update() * behaviour->weight;

		forceSum += force;
	}
	
	//scale the force vector down if it exceeds the limits of the steering behaviour manager
	if (forceSum.sqrMagnitude() > maxAcceleration * maxAcceleration)
	{
		forceSum = forceSum.normalised() * maxAcceleration;
	}

	//apply the force
	entity->velocity += forceSum * appPtr->m_deltaTime;

	//scale the velocity vector down if it exeeds the limits
	if (entity->velocity.sqrMagnitude() > maxVelocity * maxVelocity)
	{
		entity->velocity = entity->velocity.normalised() * maxVelocity;
	}

	//get the heading vector, but don't update it if the velocity vector is null
	if (entity->velocity.magnitude() > 0)
	{
		heading = entity->velocity.normalised();

		transform->rotation = heading.getRotation();
	}
}
