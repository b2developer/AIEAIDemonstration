#include "PuzzleAgent.h"
#include "Application2D.h"
#include "Transform.h"

//destructor, removes the planner and the current state
PuzzleAgent::~PuzzleAgent()
{
	delete planner;
	delete current;
}



//called once per frame
void PuzzleAgent::update()
{
	//early exit if the required planner or goal state doesn't exist
	if (planner == nullptr || planner->goalState == nullptr)
	{
		return;
	}

	presentationTimer -= appPtr->m_deltaTime;

	//only attempt to generate a new puzzle / solve an existing one if the presentation timer has run out
	if (presentationTimer > 0)
	{
		
		return;
	}

	//if there are still actions to execute
	if (plannedActions.size() == 0)
	{

		//generate a new random puzzle
		current->setRandom(randomIterations);

		//search for a solution to the new puzzle
		plannedActions = planner->solveState(current);

		//reset the presentation timer
		presentationTimer = presentationDuration;

		//set the step timer to instantly tick over
		stepTimer = 0.0f;
	}
	else
	{
		//count-down the step timer
		stepTimer -= appPtr->m_deltaTime;

		//don't make another step until the step timer runs out
		if (stepTimer > 0)
		{
			return;
		}

		stepTimer = stepDuration;

		//get the next action to be executed
		Slide* nextAction = (Slide*)plannedActions[0];

		//execute the action directly to the current state
		nextAction->executeDirect(current);

		//delete the action as it is no longer needed anymore
		delete nextAction;

		//remove the pointer from the list
		plannedActions.erase(plannedActions.begin());

		//present the solved puzzle
		if (plannedActions.size() == 0)
		{
			presentationTimer = presentationDuration;
		
		}
	}

}
