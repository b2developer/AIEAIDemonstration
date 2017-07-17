#pragma once
#include "Spawner.h"


//forward declaration
class Application2D;

/*
* class TestSpawner
* child class of Spawner
*
* test spawner, creates miscellanous objects
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class TestSpawner : public Spawner
{
public:

	//pointer to the application that is holding this
	Application2D* appPtr = nullptr;

	/*
	* TestSpawner()
	* default constructor
	*/
	TestSpawner() {}


	/*
	* ~TestSpawner()
	* default destructor
	*/
	~TestSpawner() {}


	/*
	* addComponents
	* virtual function
	*
	* adds the components to the gameobject
	*
	* @param GameObject* creation - the gameobject to add the components
	* @returns void
	*/
	void addComponents(GameObject* creation) override;

};