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
	pathfindingBotSpawner = new PathfindingBotSpawner();
	obstacleSpawner = new ObstacleSpawner();
	boidSpawner = new BoidSpawner();
	puzzleBotSpawner = new PuzzleBotSpawner();

	//link them to the application
	director->appPtr = this;
	testSpawner->appPtr = this;
	tradingBotSpawner->appPtr = this;
	pathfindingBotSpawner->appPtr = this;
	obstacleSpawner->appPtr = this;
	boidSpawner->appPtr = this;
	puzzleBotSpawner->appPtr = this;

	//create a blackboard that can hold 500 messages
	tradingBlackboard = new Blackboard(500);

	//create a blackboard that can hold 1000 messages
	boidBlackboard = new Blackboard(1000);

	//link boid related spawners to the central boid communication object
	obstacleSpawner->boidBlackboard = boidBlackboard;
	boidSpawner->boidBlackboard = boidBlackboard;

	//create the navmesh
	navMesh = new NavMesh();

	//link the navigation bot spawner to the navmesh
	pathfindingBotSpawner->navMesh = navMesh;
	
	//link the pathfinder to the game
	navMesh->appPtr = this;
	navMesh->heursticFunction = &EuclideanDistance;
	
	//load the nav mesh from file
	char navMeshPath[FILENAME_MAX];

	strcpy_s(navMeshPath, resourceFolder);
	strcat_s(navMeshPath, "meshes\\mesh2.txt");

	navMesh->load(navMeshPath);

	director->employee = tradingBotSpawner;

	//spawning code
	//-------------------------------------------------------------

	//all groups in the application
	Group tradingGroup = Group();
	Group pathfindingGroup = Group();
	Group boidGroup = Group();
	Group puzzleGroup = Group();

	tradingGroup.enabled = true;
	pathfindingGroup.enabled = false;
	boidGroup.enabled = false;
	puzzleGroup.enabled = false;

	//spawn 20 trading bots
	for (int i = 0; i < 20; i++)
	{
		tradingGroup.addGameObject(director->createGameObject());
	}

	director->employee = pathfindingBotSpawner;

	//spawn 50 pathfinding bots
	for (int i = 0; i < 50; i++)
	{
		pathfindingGroup.addGameObject(director->createGameObject());
	}

	director->employee = obstacleSpawner;

	//spawn 4 obstacles
	for (int i = 0; i < 4; i++)
	{
		boidGroup.addGameObject(director->createGameObject());
	}
	
	director->employee = boidSpawner;

	//spawn 20 boids
	for (int i = 0; i < 20; i++)
	{
		boidGroup.addGameObject(director->createGameObject());
	}

	director->employee = puzzleBotSpawner;

	//spawn a puzzle bot
	for (int i = 0; i < 2; i++)
	{
		puzzleGroup.addGameObject(director->createGameObject());
	}

	//-------------------------------------------------------------

	//initialise all game-objects
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->initialise();
	}

	//add all groups
	groups.push_back(tradingGroup);
	groups.push_back(pathfindingGroup);
	groups.push_back(boidGroup);
	groups.push_back(puzzleGroup);

	return true;
}


//runs when the game is exited
void Application2D::shutdown()
{
	//delete the director/s and the spawner/s
	delete director;
	delete testSpawner;
	delete tradingBotSpawner;
	delete pathfindingBotSpawner;
	delete obstacleSpawner;
	delete boidSpawner;
	delete puzzleBotSpawner;
	
	//delete all AI related objects
	delete tradingBlackboard;
	delete boidBlackboard;
	delete navMesh;

	//iterate through all gameobjects, deleting each one
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		delete gameObjects[i];
	}
}


//the game loop
void Application2D::update(float deltaTime)
{
	m_deltaTime = deltaTime;

	//clamp delta time about 1/30th of a second
	if (m_deltaTime > 1 / 30.0f)
	{
		m_deltaTime = 1 / 30.0f;
	}

	aie::Input* input = aie::Input::getInstance();

	//shut down the application if the escape key is pressed
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
	{
		quit();
	}

	bool groupChanged = false;

	//check if a key that switches groups was pressed
	if (input->isKeyDown(aie::INPUT_KEY_1))
	{
		activeGroup = 0;
		groupChanged = true;
	}
	else if (input->isKeyDown(aie::INPUT_KEY_2))
	{
		activeGroup = 1;
		groupChanged = true;
	}
	else if (input->isKeyDown(aie::INPUT_KEY_3))
	{
		activeGroup = 2;
		groupChanged = true;
	}
	else if (input->isKeyDown(aie::INPUT_KEY_4))
	{
		activeGroup = 3;
		groupChanged = true;
	}

	if (groupChanged)
	{
		//iterate through all groups, setting their enabled flag to the correct value
		for (size_t i = 0; i < groups.size(); i++)
		{
			groups[i].enabled = (activeGroup == (int)i);
		}
	}
	
	//update all dynamic blackboards
	tradingBlackboard->update(deltaTime);

	//update all groups
	for (size_t i = 0; i < groups.size(); i++)
	{
		groups[i].update();
	}

	//iterate through all gameobjects, updating each one
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		//store in a temporary value for performance and readability
		GameObject* gameObject = gameObjects[i];

		//only update the object if no groups are going to
		if (gameObject->groups == 0)
		{
			gameObject->update();
		}
	}

	mousePos = Vector2((float)input->getMouseX(), (float)input->getMouseY()) + cameraPos;

}


//the render routine, runs after the game
void Application2D::draw()
{
	clearScreen();

	m_renderer2D->begin();

	m_renderer2D->setCameraPos(cameraPos.x, cameraPos.y);

	m_renderer2D->setRenderColour(1, 1, 1);

	//draw all groups
	for (size_t i = 0; i < groups.size(); i++)
	{
		groups[i].draw();
	}

	//iterate through all gameobjects, drawing each one
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		//store in a temporary value for performance and readability
		GameObject* gameObject = gameObjects[i];

		//only draw the object if no groups are going to
		if (gameObject->groups == 0)
		{
			gameObject->draw();
		}
	}

	//only render the nav-mesh if the pathfinding bots are active
	if (groups[1].enabled)
	{
		navMesh->drawMesh(5.0f, 2.0f, 0.0f, 0.0f, 1.0f, 5.0f);
	}

	m_renderer2D->end();
}
