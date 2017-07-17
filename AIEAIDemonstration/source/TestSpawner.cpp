#include "TestSpawner.h"
#include "Application2D.h"

#include "Transform.h"
#include "Box.h"
#include "Renderer.h"


//adds the necessary components for a test object
void TestSpawner::addComponents(GameObject * creation)
{
	//add all components
	Transform* transform = new Transform();
	Box* box = new Box();
	Renderer* renderer = new Renderer();

	transform->translation.x = 300.0f;
	transform->translation.y = 300.0f;
	transform->scale = Vector2(1.5f, 1.0f);
	transform->rotation = 1.0f;

	//give the box some dimensions
	box->min = Vector2(-100.0f, -100.0f);
	box->max = Vector2(100.0f, 100.0f);

	char texturePath[FILENAME_MAX];

	//create the file path to the texture
	strcpy_s(texturePath, appPtr->resourceFolder);
	strcat_s(texturePath, "default.png");
	
	renderer->load(texturePath);

	//linking the components together
	renderer->box = box;
	renderer->appPtr = appPtr;
	box->transform = transform;
	transform->container = creation;

	//add the components to the list
	creation->components.push_back(transform);
	creation->components.push_back(box);
	creation->components.push_back(renderer);
	
}
