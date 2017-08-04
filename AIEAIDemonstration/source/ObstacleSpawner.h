#pragma once
#include "Spawner.h"
#include "Blackboard.h"


//forward declaration
class Application2D;

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