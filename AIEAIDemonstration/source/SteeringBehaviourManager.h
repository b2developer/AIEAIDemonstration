#pragma once
#include "Component.h"
#include "Entity.h"
#include "Blackboard.h"
#include <vector>


//forward declarations
class Transform;
class Application2D;
class SteeringBehaviour;
struct Boid;
struct Obstacle;

/*
* class SteeringBehaviourManager
* child class of Component
*
* a component that serves as a manager
* for a list of steering behaviours that combine
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class SteeringBehaviourManager : public Component
{
public:

	//pointer to the application
	Application2D* appPtr = nullptr;

	//pointer to the required transform component
	Transform* transform = nullptr;

	//pointer to the entity that will actually doing be moving
	Entity* entity = nullptr;

	//list of steering behaviours currently being used (the order matters)
	std::vector<SteeringBehaviour*> behaviours;

	//directional vector which shows which way the boid is pointing
	Vector2 heading = Vector2(1.0f, 0);

	//maximum / minimum change in position and acceleration
	float maxVelocity = 0.0f;
	float minVelocity = 0.0f;
	float minAcceleration = 0.0f;
	float maxAcceleration = 0.0f;

	//pointer to the boid enviroment blackboard
	Blackboard* enviroment = nullptr;

	//live container for other boids in the game
	std::vector<Boid> otherBoids;
	std::vector<Obstacle> obstacles;

	/*
	* SteeringBehaviourManager()
	* default constructor
	*/
	SteeringBehaviourManager() {}


	/*
	* ~SteeringBehaviourManager()
	* destructor, deletes all contained behaviours
	*/
	~SteeringBehaviourManager();


	/*
	* update
	* overrides Component's update()
	*
	* gets called once per frame, calls upon
	* all contained steering behaviours to get their
	* acceleration vectors, then combines them
	*
	* @returns void
	*/
	void update() override;

};