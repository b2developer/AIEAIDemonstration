#pragma once
#include "Spawner.h"
#include "NavMesh.h"


//forward declaration
class Application2D;

/*
* class NavigationBotSpawner
* child class of Spawner
*
* creates navigation bot objects
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class NavigationBotSpawner : public Spawner
{
public:

	//pointer to the application that is holding this
	Application2D* appPtr = nullptr;

	//navmesh to give spawned navigation bots
	NavMesh* navMesh = nullptr;

	/*
	* NavigationBotSpawner()
	* default constructor
	*/
	NavigationBotSpawner() {}


	/*
	* ~NavigationBotSpawner()
	* default destructor
	*/
	~NavigationBotSpawner() {}


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