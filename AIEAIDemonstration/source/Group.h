#pragma once
#include <vector>
#include "GameObject.h"

/*
* class Group
*
* groups game-objects together to
* be updated at the same time
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Group
{
public:

	//flag indicating if the group is active
	bool enabled = true;

	//list of gameObjects
	std::vector<GameObject*> gameObjects;

	/*
	* Group
	* default constructor
	*/
	Group() {}


	/*
	* ~Group
	* destructor, properly removes all game-objects from the group
	*/
	~Group();


	/*
	* addGameObject
	*
	* adds the game-object to the group
	*
	* @param GameObject* - pointer to the game-object to add
	* @returns void
	*/
	void addGameObject(GameObject* gameObject);


	/*
	* removeGameObject
	*
	* removes the game-object at a specific index
	* from the group
	*
	* @param size_t i - index of the game-object
	* @returns void
	*/
	void removeGameObject(size_t i);


	/*
	* contribute
	*
	* makes the group add all of it's game-objects
	* to another list of game-objects
	*
	* this is usually done to help with spawning
	*
	* @param std::vector<GameObject*>& other - reference to the other list to add to
	* @returns void
	*/
	void contribute(std::vector<GameObject*>& other);


	/*
	* initialse
	*
	* called when the game-object gets created
	*
	* @returns void
	*/
	void initialise();


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

