#include "Application2D.h"

#include <iostream>
#include <Windows.h>

//constructor
Application2D::Application2D()
{
}


//destructor
Application2D::~Application2D()
{
}


//runs when the game is told to run
bool Application2D::startup()
{
	ZeroMemory(resourceFolder, FILENAME_MAX);

	//get the path to the bin folder
	GetModuleFileName(NULL, resourceFolder, sizeof(resourceFolder) - 1);

	//find the first backslash from the right
	char* slash = strrchr(resourceFolder, '\\');

	//cut the string at the slash, leaving only the bin folder path and not the .exe
	*slash = '\0';

	//add the resources folder to the path
	strcat_s(resourceFolder, "\\resources\\");

	//create the renderer
	m_renderer2D = new aie::Renderer2D();

	//create the director and spawner/s
	director = new Director();
	testSpawner = new TestSpawner();
	tradingBotSpawner = new TradingBotSpawner();

	//link them to each other and the application
	director->appPtr = this;
	testSpawner->appPtr = this;
	tradingBotSpawner->appPtr = this;

	director->employee = tradingBotSpawner;

	//create a blackboard that can hold 500 messages
	tradingBlackboard = new Blackboard(1500);

	//create the navmesh
	navMesh = new NavMesh();
	
	//link the pathfinder to the game
	navMesh->appPtr = this;

	navMesh->heursticFunction = &EuclideanDistance;

	/*
	//generate a grid graph for pathfinding
	navMesh->createGrid(80, 60, Vector2(50.0f, 50.0f), Vector2(15.0f, 15.0f));

	int nodesToRemove = 0;

	//remove random nodes from the map
	for (int i = 0; i < nodesToRemove; i++)
	{
		//select a random node to remove
		int randNum = rand() % navMesh->data.vertices.size();

		NavNode* navNode = navMesh->data.vertices[randNum]->data;
		delete navNode;

		//remove the node
		navMesh->data.removeVertex(navMesh->data.vertices[randNum]);
	}
	*/
	
	//load the nav mesh from file
	char navMeshPath[FILENAME_MAX];

	strcpy_s(navMeshPath, resourceFolder);
	strcat_s(navMeshPath, "meshes\\mesh1.txt");

	navMesh->load(navMeshPath);

	//spawn 8 trading bots
	for (int i = 0; i < 0; i++)
	{
		gameObjects.push_back(director->createGameObject());
	}

	return true;
}


//runs when the game is exited
void Application2D::shutdown()
{
	//delete the director and the spawner/s
	delete director;
	delete testSpawner;

	delete tradingBlackboard;
	delete navMesh;

	//iterate through all gameobjects, deleting each one
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		delete gameObjects[i];
	}
}

//temp timers
float calculationTimer = 0.0f;
float calculationDuration = 1.0f;

int startNode = 0;
int endNode = 0;

//the game loop
void Application2D::update(float deltaTime)
{
	m_deltaTime = deltaTime;

	aie::Input* input = aie::Input::getInstance();

	//shut down the application if the escape key is pressed
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
	{
		quit();
	}

	tradingBlackboard->update(deltaTime);

	//iterate through all gameobjects, updating each one
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->update();
	}

	calculationTimer -= deltaTime;

	//reset the timer if it expires
	if (calculationTimer < 0.0f)
	{
		calculationTimer = calculationDuration;

		startNode = rand() % navMesh->data.vertices.size();
		endNode = rand() % navMesh->data.vertices.size();

	}
}


//the render routine, runs after the game
void Application2D::draw()
{
	clearScreen();
	m_renderer2D->setRenderColour(1, 1, 1);

	m_renderer2D->begin();

	//iterate through all gameobjects, drawing each one
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->draw();
	}

	
	navMesh->drawMesh(5.0f, 2.0f, 1.0f, 0.0f, 0.0f);
	
	std::vector<Vector2> path = navMesh->findRawPath(navMesh->data.vertices[startNode], navMesh->data.vertices[endNode]);
	path = navMesh->optimisePath(path);

	m_renderer2D->drawCircle(navMesh->data.vertices[startNode]->data->position.x, navMesh->data.vertices[startNode]->data->position.y, 5.0f);

	m_renderer2D->setRenderColour(0, 1, 0);

	m_renderer2D->drawCircle(navMesh->data.vertices[endNode]->data->position.x, navMesh->data.vertices[endNode]->data->position.y, 5.0f);

	m_renderer2D->setRenderColour(0, 1, 0);

	//iterate through the list of points, drawing it as a path
	for (size_t i = 0; i < path.size() - 1; i++)
	{
		if (path.size() == 0)
		{
			break;
		}

		if (i > 0)
		{
			m_renderer2D->drawCircle(path[i].x, path[i].y, 5.0f);
		}

		m_renderer2D->drawLine(path[i].x, path[i].y, path[i + 1].x, path[i + 1].y, 3.0f);
	}
	

	m_renderer2D->end();
}
