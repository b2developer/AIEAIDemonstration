#include "ObstacleAvoidanceBehaviour.h"
#include "SteeringBehaviourManager.h"
#include "Transform.h"
#include "Application2D.h"
#include "CollisionEngine.h"
#include "Polygon.h"

//called once per frame, gets the force to sum to the boid
Vector2 ObstacleAvoidanceBehaviour::update()
{
	float dynamicAheadDistance = aheadDistance;

	//dynamic look ahead distance based on velocity
	dynamicAheadDistance *= (sbm->entity->velocity.magnitude() / sbm->maxVelocity) * 0.5f + 0.5f;

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
	leftMat.setRotate(0.4f);

	Matrix2 rightMat = Matrix2();
	rightMat.identity();
	rightMat.setRotate(-0.4f);

	bool leftCollides = false;
	bool rightCollides = false;

	//the polygon is a line
	((Polygon*)leftPoly)->points.push_back(boidPosition);
	((Polygon*)leftPoly)->points.push_back(boidPosition + (forwardLine * leftMat));

	((Polygon*)rightPoly)->points.push_back(boidPosition);
	((Polygon*)rightPoly)->points.push_back(boidPosition + (forwardLine * rightMat));

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

	delete leftPoly;
	delete null1;

	delete rightPoly;
	delete null2;

	//early exit if there are no collisions
	if (!leftCollides || !rightCollides)
	{
		return Vector2(0, 0);
	}

	Vector2 repel = Vector2(0, 0);

	if (leftCollides && !rightCollides)
	{
		repel = sbm->heading.normal(NormalDirection::LEFT) - sbm->heading;
	}

	if (!leftCollides && rightCollides)
	{
		repel = sbm->heading.normal(NormalDirection::RIGHT) - sbm->heading;;
	}

	if (leftCollides && rightCollides)
	{
		repel = sbm->heading * -1.0f;
	}

	sbm->heading.normalise();

	return repel * sbm->maxVelocity;// -sbm->entity->velocity;
}
