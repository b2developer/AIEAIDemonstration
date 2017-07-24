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

	//create a blackboard that can hold 200 messages
	tradingBlackboard = new Blackboard(300);

	//spawn 50 game objects
	for (int i = 0; i < 5; i++)
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

	std::cout << "--------------------" << '\n';
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

	m_renderer2D->end();
}
