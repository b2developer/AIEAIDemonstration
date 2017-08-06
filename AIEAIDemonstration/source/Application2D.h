#pragma once
#include "Application.h"

#include <vector>

#include "GameObject.h"
#include "Component.h"

#include "Director.h"
#include "Spawner.h"
#include "TestSpawner.h"
#include "TradingBotSpawner.h"
#include "PathfindingBotSpawner.h"
#include "ObstacleSpawner.h"
#include "BoidSpawner.h"

#include "Blackboard.h"
#include "NavMesh.h"

#include "Input.h"
#include "Renderer2D.h"
#include "Texture.h"
#include "Audio.h"


/*
* class Application2D
* child class of aie::Application
*
* the game is contained inside this object, it is responsible
* for rendering all textures and updating all objects through a loop
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Application2D : public aie::Application
{
public:

	std::vector<GameObject*> gameObjects; //container for all gameobjects in the game

	float m_deltaTime = 0.0f; //stores the amount of time passed since the last frame
	char resourceFolder[FILENAME_MAX]; //c-string that stores the path to the folder that contains all resources

	Director* director = nullptr; //creates gameobjects with modificiations from the employed spawner
	TestSpawner* testSpawner = nullptr; //spawner for test objects
	TradingBotSpawner* tradingBotSpawner = nullptr; //spawner for trading bot objects
	PathfindingBotSpawner* pathfindingBotSpawner = nullptr; //spawner for pathfinding bot objects
	ObstacleSpawner* obstacleSpawner = nullptr; //spawner for obstacle objects
	BoidSpawner* boidSpawner = nullptr; //spawner for boid objects

	Blackboard* tradingBlackboard = nullptr; //central communication for trading agents
	Blackboard* boidBlackboard = nullptr; //communication of boids and the enviroment they are moving around in
	NavMesh* navMesh = nullptr; //navigational mesh for calculating paths

	aie::Renderer2D* m_renderer2D; //responsible for draw calls

	Vector2 mousePos = Vector2(0,0);

	/*
	* Application2D()
	* constructor, initialises the game
	*/
	Application2D();


	/*
	* ~Application2D()
	* overrides aie::Application's ~Application()
	* destructor, terminates the game
	*/
	~Application2D() override;


	/*
	* startup
	* overrides aie::Application's startup()
	*
	* attempts to initialise the application
	*
	* @returns bool - indicates if the startup succeeded
	*/
	bool startup() override;


	/*
	* shutdown
	* overrides aie::Application's shutdown()
	*
	* deletes all memory used by the program, called before the application is destroyed
	*
	* @returns void
	*/
	void shutdown() override;


	/*
	* update
	* overrides aie::Application's update(float deltaTime)
	*
	* simulates one frame of the game
	*
	* @param float deltaTime - the amount of time passed since the last frame
	* @returns void
	*/
	void update(float deltaTime) override;


	/*
	* draw
	* overrides aie::Application's draw()
	*
	* renders all required textures to the screen
	*
	* @returns void
	*/
	void draw();

};