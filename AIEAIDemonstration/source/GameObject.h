#pragma once
#include <vector>
#include "Component.h"

/*
* class GameObject
*
* groups components together to identify
* them as part of a common object
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class GameObject
{
public:

	//list of components held by this
	std::vector<Component*> components;

	/*
	* GameObject
	* default constructor
	*/
	GameObject() {}


	/*
	* ~GameObject
	* destructor, terminates all components
	*/
	~GameObject();


	/*
	* update
	*
	* called once per frame while the gameobject is active
	* 
	* @returns void
	*/
	void update();


	/*
	* draw
	*
	* called once per frame after the update, calls upon
	* all renderer components stored to render themselves
	*
	* @returns void
	*/
	void draw();
};