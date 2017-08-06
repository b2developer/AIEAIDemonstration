#include "ObstacleAvoidanceBehaviour.h"
#include "SteeringBehaviourManager.h"
#include "Transform.h"
#include "Application2D.h"
#include "CollisionEngine.h"
#include "Polygon.h"

//called once per frame, gets the force to sum to the boid
Vector2 ObstacleAvoidanceBehaviour::update()
{
	//get the position of the boid
	Vector2 boidPosition = sbm->transform->translation;

	//get a line that points directly in front of the boid
	Vector2 forwardLine = sbm->heading * aheadDistance;

	//create a polygon to test against the enviroment
	Shape* aheadPoly = new Polygon();

	//temporary transforms required by the collision engine
	Transform* null = new Transform();

	aheadPoly->transform = null;

	//the polygon is a line
	((Polygon*)aheadPoly)->points.push_back(boidPosition);
	((Polygon*)aheadPoly)->points.push_back(boidPosition + forwardLine);

	//store all colliding shapes
	std::vector<Shape*> colliders;

	//iterate through all obstacles in the world, testing for collision with the left line
	for (size_t i = 0; i < sbm->obstacles.size(); i++)
	{
		Shape* other = sbm->obstacles[i].shape;

		//test for a collision between the line and the shape
		if (COLL_ENGINE->simpleCollisionCheck(aheadPoly, other))
		{
			colliders.push_back(other);
		}
	}

	delete aheadPoly;
	delete null;

	//early exit if there are no colliders
	if (colliders.size() == 0)
	{
		return Vector2(0, 0);
	}

	Shape* closestCollider = colliders[0];
	float closestDistSquared = MAX;

	//iterate through all colliders that hit the 
	for (size_t i = 1; i < colliders.size(); i++)
	{
		//store in a temporary variable, saving performance and readability
		Shape* collider = colliders[i];

		float distSquared = (collider->transform->translation - (boidPosition + forwardLine)).sqrMagnitude();

		//if the distance to this collider
		if (distSquared < closestDistSquared)
		{
			closestCollider = collider;
			closestDistSquared = distSquared;
		}
	}

	//force away from the closest collider
	Vector2 force = (boidPosition + forwardLine * 0.5f) - closestCollider->transform->translation;
	force.normalise();

	return force * sbm->maxVelocity - sbm->entity->velocity;
}
