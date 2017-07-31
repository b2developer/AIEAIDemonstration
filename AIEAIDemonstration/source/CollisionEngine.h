#pragma once
#include "Transform.h"
#include "Shape.h"
#include "MathsLibrary.h"


#define COLL_ENGINE CollisionEngine::getInstance()

/*
* struct AdvancedCollision
*
* a singleton that performs collision checks on different types
* of shapes, and returns information about them in a variety of ways
* eg. if they collided or not, how they can be seperated
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
struct AdvancedCollision
{
	bool colliding = false; //boolean flag indicating if the collision actually happened

	Shape* A = nullptr; //the first shape in the collision
	Shape* B = nullptr; //the second shape in the colision

	Vector2 MTV = Vector2(0, 0); //vector which when applied to B will move it out of contact with A
};


/*
* class CollisionEngine
*
* a singleton that performs collision checks on different types
* of shapes, and returns information about them in a variety of ways
* eg. if they collided or not, how they can be seperated
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class CollisionEngine
{
public:

	//lee-way for binary tests (==, <=, >)
	float epsilon = 0.01f;

	/*
	* getInstance
	*
	* gets the pointer to the only instance of this
	* class, creates one if one doesn't exist
	*
	* @returns static CollisionEngine* - pointer to the instance
	*/
	static CollisionEngine* getInstance();


	/*
	* simpleCollisionCheck
	*
	* checks if two shapes are colliding
	*
	* @param Shape* A - shape that is treated as the first shape in the collision
	* @param Shape* B - shape that is treated as the second shape in the collision
	* @returns bool - indicating if a collision occurred or not
	*/
	bool simpleCollisionCheck(Shape* A, Shape* B);

	
	/*
	* advancedCollisionCheck
	*
	* checks if two shapes are colliding, but returns additional information
	* about the collision (if one occurred)
	*
	* @param Shape* A - shape that is treated as the first shape in the collision
	* @param Shape* B - shape that is treated as the second shape in the collision
	* @returns AdvancedCollision - struct that returns information on the potential collision
	*/
	AdvancedCollision advancedCollisionCheck(Shape* A, Shape* B);


	/*
	* calculateSide
	*
	* calculates which side point P is on a plane
	* formed by points A and B
	*
	* @param Vector2 A - starting point of the line
	* @param Vector2 B - ending point of the line
	* @param Vector2 P - the point to test
	* @returns int - 1 for on the left, -1 for on the right, 0 for P being on the line formed by A and B
	*/
	int calculateSide(Vector2 A, Vector2 B, Vector2 P);


	/*
	* lineCollisionCheck
	*
	* checks if two line segments formed by A-B and C-D
	* are colliding with each other
	*
	* @param Vector2 A - the first point of the first line segment
	* @param Vector2 B - the second point of the first line segment
	* @param Vector2 C - the first point of the second line segment
	* @param Vector2 D - the second point of the second line segment
	* @param float tolerance - how far the lines can be intersecting before it counts as an intersection
	* @returns bool - indicating if a collision occurred or not
	*/
	bool lineCollisionCheck(Vector2 A, Vector2 B, Vector2 C, Vector2 D, float tolerance = 0.01f);


private:

	//constructor and destructor are kept private
	//under the singleton structure

	/*
	* CollisionEngine()
	* default constructor
	*/
	CollisionEngine() {}


	/*
	* ~CollisionEngine()
	* default destructor
	*/
	~CollisionEngine() {}

};