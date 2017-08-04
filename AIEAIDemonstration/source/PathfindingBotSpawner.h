#pragma once
#include "Spawner.h"
#include "NavMesh.h"


//forward declaration
class Application2D;

/*
* class PathfindingBotSpawner
* child class of Spawner
*
* creates pathfinding bot objects
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class PathfindingBotSpawner : public Spawner
{
public:

	//pointer to the application that is holding this
	Application2D* appPtr = nullptr;

	//pointer to the mesh to navigate around
	NavMesh* navMesh = nullptr;

	/*
	* PathfindingBotSpawner()
	* default constructor
	*/
	PathfindingBotSpawner() {}


	/*
	* ~PathfindingBotSpawner()
	* default destructor
	*/
	~PathfindingBotSpawner() {}


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