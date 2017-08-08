#pragma once
#include "Spawner.h"


//forward declaration
class Application2D;

/*
* class PuzzleBotSpawner
* child class of Spawner
*
* creates puzzle bot objects
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class PuzzleBotSpawner : public Spawner
{
public:

	//pointer to the application that is holding this
	Application2D* appPtr = nullptr;

	/*
	* PuzzleBotSpawner()
	* default constructor
	*/
	PuzzleBotSpawner() {}


	/*
	* ~PuzzleBotSpawner()
	* default destructor
	*/
	~PuzzleBotSpawner() {}


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