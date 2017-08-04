#include "PathfindingBotSpawner.h"
#include "Application2D.h"

#include "Transform.h"
#include "Box.h"
#include "Circle.h"
#include "Renderer.h"
#include "PathfindingAgent.h"

//adds the necessary components for a trading bot object
void PathfindingBotSpawner::addComponents(GameObject * creation)
{
	//add all components
	Transform* transform = new Transform();
	Circle* circle = new Circle();
	Box* box = new Box();
	Renderer* renderer = new Renderer();
	Agent* pathfindingAgent = new PathfindingAgent();

	transform->scale = Vector2(1.0f, 1.0f);
	transform->rotation = 0.0f;

	//give the circle a random radius
	circle->radius = ((rand() % 700) + 300) * (30.0f / 1000.0f);

	//give the NavMesh and shoulder radius to the agent
	((PathfindingAgent*)pathfindingAgent)->navMesh = navMesh;
	((PathfindingAgent*)pathfindingAgent)->radius = circle->radius;

	//construct the box to match the circle
	box->min = Vector2(-circle->radius, -circle->radius);
	box->max = Vector2(circle->radius, circle->radius);

	char texturePath[FILENAME_MAX];

	//load the navigation bot texture for the renderer
	strcpy_s(texturePath, appPtr->resourceFolder);
	strcat_s(texturePath, "textures\\navBot.png");
	renderer->load(texturePath);

	//linking the components together
	renderer->box = box;
	renderer->appPtr = appPtr;
	circle->transform = transform;
	box->transform = transform;
	pathfindingAgent->appPtr = appPtr;
	pathfindingAgent->transform = transform;

	//link the components to the game object
	transform->container = creation;
	circle->container = creation;
	box->container = creation;
	renderer->container = creation;
	pathfindingAgent->container = creation;

	//add the components to the list
	creation->components.push_back(transform);
	creation->components.push_back(circle);
	creation->components.push_back(box);
	creation->components.push_back(renderer);
	creation->components.push_back(pathfindingAgent);

}
