#pragma once
#include "GameObject.h"
#include "Spawner.h"


//forward declaration
class Application2D;

/*
* class Director
*
* commands a spawner to set up a newly created GameObject
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Director
{
public:

	//pointer to the application that is holding this
	Application2D* appPtr = nullptr;
	
	//the spawner that the director is commanding
	Spawner* employee = nullptr;

	/*
	* Director()
	* default constructor
	*/
	Director() {}


	/*
	* ~Director()
	* default destructor
	*/
	~Director() {}


	/*
	* createGameObject
	*
	* allocates a new gameobject, tells the spawner
	* to generate components for it
	*
	* @returns GameObject* - the gameobject created, filled with additional components from the spawner
	*/
	GameObject* createGameObject();
};