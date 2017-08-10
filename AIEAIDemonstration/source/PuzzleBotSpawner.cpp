#include "PuzzleBotSpawner.h"
#include "Application2D.h"

#include "Transform.h"
#include "PuzzleRenderer.h"
#include "PuzzleAgent.h"
#include "Planner.h"
#include "Puzzle.h"

//adds the necessary components for a puzzle bot object
void PuzzleBotSpawner::addComponents(GameObject * creation)
{
	//add all components
	Transform* transform = new Transform();
	Box* box = new Box();
	Renderer* puzzleRenderer = new PuzzleRenderer();
	Agent* puzzleAgent = new PuzzleAgent();

	transform->translation = Vector2(200.0f + 300.0f * spawnCount, 200.0f);
	transform->scale = Vector2(1.0f, 1.0f);
	transform->rotation = 0.0f;

	//construct the box
	box->min = Vector2(0.0f, 0.0f);
	box->max = Vector2(200.0f, 200.0f);

	char texturePath[FILENAME_MAX];

	//load the navigation bot texture for the renderer
	strcpy_s(texturePath, appPtr->resourceFolder);
	
	//load different textures depending on the amount of puzzle bots spawned
	if (spawnCount == 0)
	{
		strcat_s(texturePath, "textures\\kool-aid-man.png");
	}
	else if (spawnCount == 1)
	{
		strcat_s(texturePath, "textures\\apple.png");
	}

	puzzleRenderer->load(texturePath);

	((PuzzleRenderer*)puzzleRenderer)->movementSpeed = 12.0f;

	//give the puzzle agent required data
	((PuzzleAgent*)puzzleAgent)->planner = new Planner();
	((PuzzleAgent*)puzzleAgent)->puzzleISize = 3;
	((PuzzleAgent*)puzzleAgent)->puzzleJSize = 3;
	((PuzzleAgent*)puzzleAgent)->randomIterations = 1000;

	((PuzzleAgent*)puzzleAgent)->presentationDuration = 3.0f;
	((PuzzleAgent*)puzzleAgent)->stepDuration= 0.5f;

	//create a goal state for the planner
	PlannerState* goalState = new Puzzle(Vector2((float)((PuzzleAgent*)puzzleAgent)->puzzleJSize, (float)((PuzzleAgent*)puzzleAgent)->puzzleISize));
	((Puzzle*)goalState)->setGoal();
	((PuzzleAgent*)puzzleAgent)->planner->goalState = goalState;

	//create a random state for the planner to work on
	Puzzle* randomState = new Puzzle(Vector2((float)((PuzzleAgent*)puzzleAgent)->puzzleJSize, (float)((PuzzleAgent*)puzzleAgent)->puzzleISize));
	randomState->setGoal();

	((PuzzleAgent*)puzzleAgent)->current = randomState;

	//give the renderer the puzzle
	((PuzzleRenderer*)puzzleRenderer)->puzzle = randomState;

	//linking the components together
	box->transform = transform;
	((PuzzleRenderer*)puzzleRenderer)->transform = transform;
	puzzleRenderer->appPtr = appPtr;
	puzzleRenderer->box = box;
	puzzleAgent->appPtr = appPtr;

	//link the components to the game object
	transform->container = creation;
	box->container = creation;
	puzzleRenderer->container = creation;
	puzzleAgent->container = creation;

	//add the components to the list
	creation->components.push_back(transform);
	creation->components.push_back(box);
	creation->components.push_back(puzzleRenderer);
	creation->components.push_back(puzzleAgent);

	//increment the spawn count
	spawnCount++;
}
