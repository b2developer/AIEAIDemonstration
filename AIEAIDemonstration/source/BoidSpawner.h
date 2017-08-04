#pragma once
#include "Spawner.h"


//forward declarations
class Application2D;
class SteeringBehaviourManager;

//type for boids that can spawn
enum class BoidMode
{
	MOUSE_FOLLOWER,
	PURSUER,
	AVOID_BUT_FOLLOW,
};

/*
* class BoidSpawner
* child class of Spawner
*
* creates boid objects
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class BoidSpawner : public Spawner
{
public:

	//pointer to the application that is holding this
	Application2D* appPtr = nullptr;

	//modifies the type of boids spawned
	BoidMode mode = BoidMode::MOUSE_FOLLOWER;

	//pointer to the boid that was created in the last call
	SteeringBehaviourManager* previousBoid = nullptr;

	/*
	* BoidSpawner()
	* default constructor
	*/
	BoidSpawner() {}


	/*
	* ~BoidSpawner()
	* default destructor
	*/
	~BoidSpawner() {}


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