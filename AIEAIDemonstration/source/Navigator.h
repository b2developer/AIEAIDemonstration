#pragma once
#include "Component.h"
#include "NavMesh.h"


//forward declarations
class Transform;
class Application2D;

/*
* class Navigator
* child class of Component
*
* a component that serves as an entity that
* can navigate around complex obstacles by requesting
* and following paths from a NavMesh
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Navigator : public Component
{
public:

	//pointer to the application
	Application2D* appPtr = nullptr;

	//pointer to the required transform component
	Transform* transform = nullptr;

	//pointer to the required navMesh for navigation
	NavMesh* navMesh = nullptr;

	//list of points to follow to reach the destination
	std::vector<Vector2> path;

	//units moved per second
	float movementSpeed = 100.0f;

	/*
	* Navigator()
	* default constructor
	*/
	Navigator() {}


	/*
	* ~Navigator()
	* default destructor
	*/
	~Navigator() {}


	/*
	* update
	* overrides Component's update()
	*
	* gets called once per frame, updates the position
	* and path requests of the navigator
	*
	* @returns void
	*/
	void update() override;


	/*
	* setTarget
	*
  	* requests a path from the NavMesh
	* 
	* @param Vector2 target - the vector to navigate to
	* @returns void
	*/
	void setTarget(Vector2 target);

};