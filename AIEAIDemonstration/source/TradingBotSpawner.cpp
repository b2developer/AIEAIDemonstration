#include "TradingBotSpawner.h"
#include "Application2D.h"

#include "Transform.h"
#include "Box.h"
#include "Renderer.h"
#include "TradingAgent.h"

#include "SearchState.h"
#include "TradingTransition.h"
#include "GotoState.h"
#include "RangeTransition.h"
#include "ExchangeState.h"
#include "ExchangedTransition.h"


const int STARTING_INVENTORY_SIZE = 10;

//adds the necessary components for a test object
void TradingBotSpawner::addComponents(GameObject * creation)
{
	//add all components
	Transform* transform = new Transform();
	Box* box = new Box();
	Renderer* renderer = new Renderer();
	Agent* tradingAgent = new TradingAgent();

	transform->translation.x = rand() % 500;
	transform->translation.y = rand() % 500;

	//random position to travel back to after a trade
	((TradingAgent*)tradingAgent)->origin = transform->translation;

	transform->scale = Vector2(1.0f, 1.0f);
	transform->rotation = 0.0f;

	//give the box some dimensions
	box->min = Vector2(-25.0f, -25.0f);
	box->max = Vector2(25.0f, 25.0f);

	char texturePath[FILENAME_MAX];

	//create the file path to the texture
	strcpy_s(texturePath, appPtr->resourceFolder);
	strcat_s(texturePath, "bot.png");

	renderer->load(texturePath);

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
	renderer->appPtr = appPtr;
	box->transform = transform;
	transform->container = creation;
	tradingAgent->appPtr = appPtr;
	tradingAgent->transform = transform;

	FSM->setLinks();

	//link the components to the game object
	transform->container = creation;
	box->container = creation;
	renderer->container = creation;
	tradingAgent->container = creation;

	//add the components to the list
	creation->components.push_back(transform);
	creation->components.push_back(box);
	creation->components.push_back(renderer);
	creation->components.push_back(tradingAgent);

}
