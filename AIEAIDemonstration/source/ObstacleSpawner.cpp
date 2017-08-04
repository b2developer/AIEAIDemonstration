#include "ObstacleSpawner.h"
#include "Application2D.h"

#include "Transform.h"
#include "Box.h"
#include "Circle.h"
#include "Polygon.h"

//adds the necessary components for a test object
void ObstacleSpawner::addComponents(GameObject * creation)
{
	//add all components
	Transform* transform = new Transform();

	transform->translation.x = 300.0f;
	transform->translation.y = 300.0f;
	transform->scale = Vector2(1.5f, 1.0f);
	transform->rotation = 1.0f;

	//linking the components together
	transform->container = creation;

	//link the components to the game object
	transform->container = creation;

	//add the components to the list
	creation->components.push_back(transform);

}
