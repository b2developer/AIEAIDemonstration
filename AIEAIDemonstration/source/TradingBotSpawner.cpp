#include "TradingBotSpawner.h"
#include "Application2D.h"

#include "Transform.h"
#include "Box.h"
#include "Renderer.h"
#include "TradingAgent.h"

#include "SearchState.h"

//adds the necessary components for a test object
void TradingBotSpawner::addComponents(GameObject * creation)
{
	//add all components
	Transform* transform = new Transform();
	Box* box = new Box();
	Renderer* renderer = new Renderer();
	Agent* tradingAgent = new TradingAgent();

	transform->translation.x = 300.0f;
	transform->translation.y = 300.0f;
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

	//build the FSM of the trading agent
	State* searchState = new SearchState();
	((TradingAgent*)tradingAgent)->FSM->data.addVertex(searchState);

	//set the starting vertex (state)
	((TradingAgent*)tradingAgent)->FSM->currentVertex = ((TradingAgent*)tradingAgent)->FSM->data.vertices[0];

	//linking the components together
	renderer->box = box;
	renderer->appPtr = appPtr;
	box->transform = transform;
	transform->container = creation;
	tradingAgent->appPtr = appPtr;
	tradingAgent->transform = transform;
	((TradingAgent*)tradingAgent)->FSM->setLinks();

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
