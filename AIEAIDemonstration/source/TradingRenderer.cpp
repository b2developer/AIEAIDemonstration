#include "TradingRenderer.h"
#include "Application2D.h"
#include "Transform.h"

//destructor, removes the animation
TradingRenderer::~TradingRenderer()
{
	//properly deallocate stored textures
	for (size_t i = 0; i < 3; i++)
	{
		delete blinkAnimation[i];
	}

	//properly deallocate the item textures
	for (size_t i = 0; i < itemTextures.size(); i++)
	{
		delete itemTextures[i];
	}

	texture = nullptr;
}


//called once per frame during the draw call
void TradingRenderer::update()
{

	//has the idle timer expired
	if (idleTimer <= 0.0f)
	{
		//count down the blink timer
		blinkTimer -= appPtr->m_deltaTime;

		//has the blink timer expired
		if (blinkTimer <= 0.0f)
		{
			//reset the blink timer
			blinkTimer = blinkDuration;
			
			//increment the animation
			strip++;

			//has the animation looped
			if (strip >= (int)blinkAnimation.size())
			{
				strip = 0;

				//choose a random amount of seconds between idleMax and idleMin to a thousandth of a second
				float randTime = ((rand() % 1000) * ((int)(idleMax * 1000.0f) - (int)(idleMin * 1000.0f)) + (int)(idleMin * 1000.0f)) / 1000000.0f;

				//set the idle timer
				idleTimer = randTime;
			}
		}
	}
	else
	{
		//count down the idle timer
		idleTimer -= appPtr->m_deltaTime;
	}

	texture = blinkAnimation[strip];

	//renderer requires a texture, region to render to and a transform
	if (texture == nullptr || box == nullptr || box->transform == nullptr)
	{
		return;
	}

	Vector2 mid = box->transform->translation;

	//create a rotation matrix
	Matrix2 rM = Matrix2();
	rM.identity();

	//set the rotation of the matrix
	rM.setRotate(box->transform->rotation);

	//get the bottom left corner of the box (after all transformations)
	Vector2 g_bl = box->min * rM + mid;

	//render the texture at the region specified by the box and it's associated transform
	appPtr->m_renderer2D->drawSprite(texture, g_bl.x, g_bl.y, (box->max.x - box->min.x) * box->transform->scale.x, (box->max.y - box->min.y) * box->transform->scale.y, box->transform->rotation, -1.0f, 0.0f, 0.0f);

	//if the trading agent is trading
	if (tradingAgent->other != nullptr)
	{
		appPtr->m_renderer2D->drawLine(tradingAgent->transform->translation.x, tradingAgent->transform->translation.y, tradingAgent->other->transform->translation.x, tradingAgent->other->transform->translation.y, 2.0f);
	}

	//iterate through every inventory item, rendering each one
	for (size_t i = 0; i < tradingAgent->inventory.size(); i++)
	{
		ObjectType object = tradingAgent->inventory[i];

		Vector2 span = itemBox->max - itemBox->min;

		//draw the item in the correct place, offset to the left to centre the entire item list
		appPtr->m_renderer2D->drawSprite(itemTextures[object], 

		g_bl.x + span.x * box->transform->scale.x * (i - tradingAgent->inventory.size() / 2.0f + 1), //x coordinate of the item
		g_bl.y - span.y * box->transform->scale.y, //y coordinate of the item
		span.x * box->transform->scale.x, //x scale of the item
		span.y * box->transform->scale.y, //y scale of the item
		box->transform->rotation, -1.0f, 0.0f, 0.0f);
	}
}
