#pragma once
#include "Spawner.h"


//forward declaration
class Application2D;

/*
* class TradingBotSpawner
* child class of Spawner
*
* test spawner, creates trading bot objects
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class TradingBotSpawner : public Spawner
{
public:

	//pointer to the application that is holding this
	Application2D* appPtr = nullptr;

	/*
	* TradingBotSpawner()
	* default constructor
	*/
	TradingBotSpawner() {}


	/*
	* ~TradingBotSpawner()
	* default destructor
	*/
	~TradingBotSpawner() {}


	/*
	* addComponents
	* virtual function
	*
	* adds the components to the gameobject
	*
	* @param GameObject* creation - the gameobject to add the components
	* @returns void
	*/
	virtual void addComponents(GameObject* creation) override;

};