#include "TradingBotSpawner.h"
#include "Application2D.h"

#include "Transform.h"
#include "Box.h"
#include "Renderer.h"
#include "Navigator.h"


//adds the necessary components for a navigation bot object
void NavigationBotSpawner::addComponents(GameObject * creation)
{
	//add all components
	Transform* transform = new Transform();
	Box* box = new Box();
	Renderer* renderer = new Renderer();
	Navigator* navigator = new Navigator();

	transform->translation.x = (float)(rand() % 1000);
	transform->translation.y = (float)(rand() % 1000);

	transform->scale = Vector2(1.0f, 1.0f);
	transform->rotation = 0.0f;

	//give the box some dimensions
	box->min = Vector2(-25.0f, -25.0f);
	box->max = Vector2(25.0f, 25.0f);

	char texturePath[FILENAME_MAX];

	//load the blink animation for the bot
	strcpy_s(texturePath, appPtr->resourceFolder);
	strcat_s(texturePath, "textures\\navBot.png");
	renderer->load(texturePath);

	navigator->navMesh = navMesh;

	//linking the components together
	renderer->box = box;
	renderer->appPtr = appPtr;
	box->transform = transform;
	navigator->appPtr = appPtr;
	navigator->transform = transform;

	//link the components to the game object
	transform->container = creation;
	box->container = creation;
	renderer->container = creation;
	navigator->container = creation;

	//add the components to the list
	creation->components.push_back(transform);
	creation->components.push_back(box);
	creation->components.push_back(renderer);
	creation->components.push_back(navigator);

}
