#pragma once
#include "Spawner.h"
#include "Blackboard.h"


//forward declarations
class Application2D;
class Shape;

//the amount of items in the enum ShapeType
const int SHAPE_TYPE_LENGTH = 3;

//type of shape for the obstacle
enum ShapeType
{
	CIRCLE = 0,
	BOX = 1,
	POLYGON = 2,
};


/*
* struct Obstacle
*
* data structure for obstacles
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
struct Obstacle
{
	Shape* shape = nullptr;
};


/*
* class ObstacleSpawner
* child class of Spawner
*
* test spawner, creates objects that boids must avoid
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class ObstacleSpawner : public Spawner
{
public:

	//pointer to the application that is holding this
	Application2D* appPtr = nullptr;

	//blackboard that tells the boid about obstacles and other boids
	Blackboard* boidBlackboard = nullptr;

	/*
	* ObstacleSpawner()
	* default constructor
	*/
	ObstacleSpawner() {}


	/*
	* ~ObstacleSpawner()
	* default destructor
	*/
	~ObstacleSpawner() {}


	/*
	* addComponents
	* virtual function
	*
	* adds the components to the gameobject
	*
	* @param GameObject* creation - the gameobject to add the components
	* @returns void
	*/
	virtual void addComponents(GameObject* creation) override;

};