#pragma once


//forward decleration of the GameObject
class GameObject;

/*
* class Component
*
* base object that represents a form of functionality 
* that can be applied to a gameobject
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Component
{
public:

	//pointer to the gameobject that is holding thiss
	GameObject* container = nullptr;

	/*
	* Component()
	* default constructor
	*/
	Component() {}


	/*
	* ~Component()
	* virtual function
	* default destructor
	*/
	virtual ~Component() {}


	/*
	* initialise
	* virtual function
	*
	* gets called once when the game-object is initialised
	*
	* @returns void
	*/
	virtual void initialise() {};


	/*
	* update
	* abstract function
	*
	* gets called once per frame, or whenever
	* necessary for special component types
	*
	* @returns void
	*/
	virtual void update() = 0;
};