#include "ObstacleAvoidanceBehaviour.h"
#include "SteeringBehaviourManager.h"
#include "Transform.h"
#include "Application2D.h"
#include "CollisionEngine.h"
#include "Polygon.h"

//called once per frame, gets the force to sum to the boid
Vector2 ObstacleAvoidanceBehaviour::update()
{
	//scales will be applied to this distance copy
	float dynamicAheadDistance = aheadDistance;

	//scaling applied to the feeler distances
	float dynamicScale = fminf((sbm->entity->velocity.sqrMagnitude() / sbm->maxVelocity) * dynamicScaling, 1.0f);

	//scaling applied to the force vector
	float heavyDynamicScale = (sbm->entity->velocity.magnitude() / sbm->maxVelocity) * heavyDynamicScaling + heavyDynamicOffset;

	//dynamic look ahead distance based on velocity
	dynamicAheadDistance *= dynamicScale;

	//get the position of the boid
	Vector2 boidPosition = sbm->transform->translation;

	//get a line that points directly in front of the boid
	Vector2 forwardLine = sbm->heading * dynamicAheadDistance;

	//create two polygons to test against the enviroment
	Shape* leftPoly = new Polygon();
	Shape* rightPoly = new Polygon();

	//temporary transforms required by the collision engine
	Transform* null1 = new Transform();
	Transform* null2 = new Transform();

	leftPoly->transform = null1;
	rightPoly->transform = null2;

	Matrix2 leftMat = Matrix2();
	leftMat.identity();
	leftMat.setRotate(feelerAngle * DEG2RAD);

	Matrix2 rightMat = Matrix2();
	rightMat.identity();
	rightMat.setRotate(-feelerAngle  * DEG2RAD);

	bool leftCollides = false;
	bool rightCollides = false;

	//the polygon is a line
	((Polygon*)leftPoly)->points.push_back(boidPosition + (forwardLine * leftMat));
	((Polygon*)leftPoly)->points.push_back(boidPosition);

	((Polygon*)rightPoly)->points.push_back(boidPosition + (forwardLine * rightMat));
	((Polygon*)rightPoly)->points.push_back(boidPosition);

	//store all colliding shapes
	std::vector<Shape*> colliders;

	//iterate through all obstacles in the world, testing for collision with the left line
	for (size_t i = 0; i < sbm->obstacles.size(); i++)
	{
		Shape* other = sbm->obstacles[i].shape;

		//test for a collision between the left line and the shape
		if (!leftCollides && COLL_ENGINE->simpleCollisionCheck(leftPoly, other))
		{
			leftCollides = true;
		}

		//test for a collision between the right line and the shape
		if (!rightCollides && COLL_ENGINE->simpleCollisionCheck(rightPoly, other))
		{
			rightCollides = true;
		}
	}

	//remove the polygons and transforms used for testing
	delete leftPoly;
	delete null1;

	delete rightPoly;
	delete null2;

	//early exit if there are no collisions
	if (!leftCollides && !rightCollides)
	{
		return Vector2(0, 0);
	}

	Vector2 repel = Vector2(0, 0);

	//if only the left feeler collided, move to the right
	if (leftCollides && !rightCollides)
	{
		repel = sbm->heading.normal(NormalDirection::RIGHT) * singularMultiplier;
	}

	//if only the right feeler collided, move to the left
	if (!leftCollides && rightCollides)
	{
		repel = sbm->heading.normal(NormalDirection::LEFT) * singularMultiplier;
	}

	//if both collided, move backwards
	if (leftCollides && rightCollides)
	{
		repel = sbm->heading * -1.0f;
	}

	return repel * sbm->maxVelocity * heavyDynamicScale;
}
