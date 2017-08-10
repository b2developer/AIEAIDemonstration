#include "Group.h"

//destructor, properly removes all game-objects from the group
Group::~Group()
{
	//iterate through all game-objects, subtracting from the group counter
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->groups--;
	}
}

//adds the game-object to the list
void Group::addGameObject(GameObject * gameObject)
{
	gameObjects.push_back(gameObject);
	gameObject->groups++;
}

//removes the game-object at the specific index
void Group::removeGameObject(size_t i)
{
	gameObjects[i]->groups--;
	gameObjects.erase(gameObjects.begin() + i);
}

//adds all of it's game-objects to another list
void Group::contribute(std::vector<GameObject*>& other)
{
	//iterate through all game-objects, adding each to the other list
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		other.push_back(gameObjects[i]);
	}
}

//initialises all game-objects
void Group::initialise()
{
	//iterate through all game-objects, initialising them
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->initialise();
	}
}

//updates all game-objects
void Group::update()
{
	//early exit if the group is in-active
	if (!enabled)
	{
		return;
	}

	//iterate through all game-objects, updating them
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->update();
	}
}

//draws all game-objects
void Group::draw()
{
	//early exit if the group is in-active
	if (!enabled)
	{
		return;
	}

	//iterate through all game-objects, rendering them
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->draw();
	}
}
