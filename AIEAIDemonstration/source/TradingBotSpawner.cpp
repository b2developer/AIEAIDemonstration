#include "TradingBotSpawner.h"
#include "Application2D.h"

#include "Transform.h"
#include "Box.h"
#include "TradingRenderer.h"
#include "TradingAgent.h"

#include "SearchState.h"
#include "TradingTransition.h"
#include "GotoState.h"
#include "RangeTransition.h"
#include "ExchangeState.h"
#include "ExchangedTransition.h"


const int STARTING_INVENTORY_SIZE = 4;

//adds the necessary components for a trading bot object
void TradingBotSpawner::addComponents(GameObject * creation)
{
	//add all components
	Transform* transform = new Transform();
	Box* box = new Box();
	Box* itemBox = new Box();
	TradingRenderer* renderer = new TradingRenderer();
	Agent* tradingAgent = new TradingAgent();

	transform->translation.x = (float)(rand() % 1000);
	transform->translation.y = (float)(rand() % 1000);

	//random position to travel back to after a trade
	((TradingAgent*)tradingAgent)->origin = transform->translation;

	transform->scale = Vector2(1.0f, 1.0f);
	transform->rotation = 0.0f;

	//give the box some dimensions
	box->min = Vector2(-25.0f, -25.0f);
	box->max = Vector2(25.0f, 25.0f);

	itemBox->min = Vector2(-15.0f, -15.0f);
	itemBox->max = Vector2(15.0f, 15.0f);

	char texturePath[FILENAME_MAX];

	//load the blink animation for the bot
	strcpy_s(texturePath, appPtr->resourceFolder);
	strcat_s(texturePath, "textures\\bot.png");
	renderer->load(texturePath);
	renderer->blinkAnimation.push_back(renderer->texture);
	renderer->texture = nullptr;

	//blink1.png
	strcpy_s(texturePath, appPtr->resourceFolder);
	strcat_s(texturePath, "textures\\bot_blink1.png");
	renderer->load(texturePath);
	renderer->blinkAnimation.push_back(renderer->texture);
	renderer->texture = nullptr;

	//blink2.png
	strcpy_s(texturePath, appPtr->resourceFolder);
	strcat_s(texturePath, "textures\\bot_blink2.png");
	renderer->load(texturePath);
	renderer->blinkAnimation.push_back(renderer->texture);
	renderer->texture = nullptr;

	//the rest of the animation is loaded textures
	renderer->blinkAnimation.push_back(renderer->blinkAnimation[2]);
	renderer->blinkAnimation.push_back(renderer->blinkAnimation[1]);

	//apple.png
	strcpy_s(texturePath, appPtr->resourceFolder);
	strcat_s(texturePath, "textures\\apple.png");
	renderer->load(texturePath);
	renderer->itemTextures.push_back(renderer->texture);
	renderer->texture = nullptr;

	//banana.png
	strcpy_s(texturePath, appPtr->resourceFolder);
	strcat_s(texturePath, "textures\\banana.png");
	renderer->load(texturePath);
	renderer->itemTextures.push_back(renderer->texture);
	renderer->texture = nullptr;

	//cherry.png
	strcpy_s(texturePath, appPtr->resourceFolder);
	strcat_s(texturePath, "textures\\cherry.png");
	renderer->load(texturePath);
	renderer->itemTextures.push_back(renderer->texture);
	renderer->texture = nullptr;

	//orange.png
	strcpy_s(texturePath, appPtr->resourceFolder);
	strcat_s(texturePath, "textures\\orange.png");
	renderer->load(texturePath);
	renderer->itemTextures.push_back(renderer->texture);
	renderer->texture = nullptr;

	renderer->tradingAgent = ((TradingAgent*)tradingAgent);
	renderer->itemBox = itemBox;

	//give the trading agent it's FSM and Blackboard
	((TradingAgent*)tradingAgent)->FSM = new FiniteStateMachine();
	((TradingAgent*)tradingAgent)->FSM->agent = tradingAgent;
	((TradingAgent*)tradingAgent)->tradingBlackboard = appPtr->tradingBlackboard;

	//give the trading agent an inventory
	for (int i = 0; i < STARTING_INVENTORY_SIZE; i++)
	{
		//add a random item to the inventory
		((TradingAgent*)tradingAgent)->inventory.push_back((ObjectType)(rand() % OBJECT_SIZE));
	}

	FiniteStateMachine* FSM = ((TradingAgent*)tradingAgent)->FSM;

	//build the FSM of the trading agent
	State* searchState = new SearchState();
	FSM->data.addVertex(searchState);

	State* gotoState = new GotoState();
	FSM->data.addVertex(gotoState);

	Transition* tradingTransition = new TradingTransition();
	FSM->data.createConnection(FSM->data.vertices[0], FSM->data.vertices[1]);

	State* exchangeState = new ExchangeState();
	FSM->data.addVertex(exchangeState);

	Transition* rangeTransition = new RangeTransition();
	FSM->data.createConnection(FSM->data.vertices[1], FSM->data.vertices[2]);

	State* homeState = new GotoState();
	FSM->data.addVertex(homeState);

	Transition* exchangedTransition = new ExchangedTransition();
	FSM->data.createConnection(FSM->data.vertices[2], FSM->data.vertices[3]);

	Transition* homeTransition = new RangeTransition();
	FSM->data.createConnection(FSM->data.vertices[3], FSM->data.vertices[0]);


	FSM->data.vertices[0]->edges[0].value = tradingTransition;
	FSM->data.vertices[1]->edges[0].value = rangeTransition;
	FSM->data.vertices[2]->edges[0].value = exchangedTransition;
	FSM->data.vertices[3]->edges[0].value = homeTransition;

	//set the starting vertex (state)
	((TradingAgent*)tradingAgent)->FSM->currentVertex = ((TradingAgent*)tradingAgent)->FSM->data.vertices[0];

	//linking the components together
	renderer->box = box;
	renderer->itemBox = itemBox;
	renderer->appPtr = appPtr;
	box->transform = transform;
	tradingAgent->appPtr = appPtr;
	tradingAgent->transform = transform;

	FSM->setLinks();

	//link the components to the game object
	transform->container = creation;
	box->container = creation;
	itemBox->container = creation;
	renderer->container = creation;
	tradingAgent->container = creation;

	//add the components to the list
	creation->components.push_back(transform);
	creation->components.push_back(box);
	creation->components.push_back(itemBox);
	creation->components.push_back(renderer);
	creation->components.push_back(tradingAgent);

}
