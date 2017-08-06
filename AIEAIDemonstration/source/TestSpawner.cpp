#include "TestSpawner.h"
#include "Application2D.h"

#include "Transform.h"
#include "Box.h"
#include "Renderer.h"
#include "Entity.h"

//adds the necessary components for a test object
void TestSpawner::addComponents(GameObject * creation)
{
	//add all components
	Transform* transform = new Transform();
	Box* box = new Box();
	Renderer* renderer = new Renderer();
	Entity* entity = new Entity();

	transform->translation.x = 300.0f;
	transform->translation.y = 300.0f;
	transform->scale = Vector2(1.0f, 1.0f);
	transform->rotation = 1.0f;

	//give the box some dimensions
	box->min = Vector2(-100.0f, -100.0f);
	box->max = Vector2(100.0f, 100.0f);

	entity->velocity = Vector2(100, 0);

	char texturePath[FILENAME_MAX];

	//create the file path to the texture
	strcpy_s(texturePath, appPtr->resourceFolder);
	strcat_s(texturePath, "default.png");
	
	renderer->load(texturePath);

	//linking the components together
	renderer->box = box;
	renderer->appPtr = appPtr;
	entity->appPtr = appPtr;
	entity->transform = transform;
	box->transform = transform;
	transform->container = creation;

	//link the components to the game object
	transform->container = creation;
	box->container = creation;
	renderer->container = creation;
	entity->container = creation;

	//add the components to the list
	creation->components.push_back(transform);
	creation->components.push_back(box);
	creation->components.push_back(renderer);
	creation->components.push_back(entity);

}
