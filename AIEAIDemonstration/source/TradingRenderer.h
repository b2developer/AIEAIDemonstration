#pragma once
#include "Renderer.h"
#include "TradingAgent.h"
#include <vector>

//forward declaration
class Application2D;

#define LARGE_NUMBER 10000000

/*
* class TradingRenderer
* child class of Renderer
*
* a component that stores a texture
* and renders it when required
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class TradingRenderer : public Renderer
{
public:

	//pointer to the game
	Application2D* appPtr = nullptr;

	//the region to draw the texture to
	Box* box = nullptr;

	//the region to draw a single item texture to
	Box* itemBox = nullptr;

	//agent component that that belongs to the same gameobject
	TradingAgent* tradingAgent = nullptr;

	//the texture data to draw
	std::vector<aie::Texture*> blinkAnimation;
	std::vector<aie::Texture*> itemTextures;

	//blinking timer and duration, the animation will play through this
	int strip = LARGE_NUMBER;
	float blinkTimer = 0.0f;
	float blinkDuration = 0.04f;

	//timer for a period in which the bot isn't blinking
	float idleTimer = 0.0f;

	//maximum time in which a blink cannot occur
	float idleMax = 4.0f;

	//minumum time in which a blink cannot occur
	float idleMin = 0.4f;

	/*
	* TradingRenderer()
	* default constructor
	*/
	TradingRenderer() {}


	/*
	* ~TradingRenderer()
	* destructor, removes additional textures from the library
	*/
	~TradingRenderer();


	/*
	* update
	* overrides Renderer's update()
	*
	* gets called once per frame when the
	* gameobject is called upon to be drawn
	*
	* @returns void
	*/
	void update() override;
};