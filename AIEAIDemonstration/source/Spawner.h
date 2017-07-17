#pragma once
#include "GameObject.h"


/*
* class Spawner
*
* base class for an object that decides how another object
* gets built using by modifying a procedure 
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Spawner
{
public:

	/*
	* Spawner()
	* default constructor
	*/
	Spawner() {}


	/*
	* ~Spawner()
	* virtual function
	* default destructor
	*/
	virtual ~Spawner() {}


	/*
	* addComponents
	* abstract function
	*
	* adds the components to the gameobject
	*
	* @param GameObject* creation - the gameobject to add the components
	* @returns void
	*/
	virtual void addComponents(GameObject* creation) = 0;

};