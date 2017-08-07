#include "Planner.h"

//destructor, removes the goal state
Planner::~Planner()
{
}

//uses A* to determine a set of actions that turn the orignal state into the goal state
//T - derived state, U - derived action
std::vector<PlannerAction*> Planner::solveState(PlannerState * original)
{
	//lists to remember created objects and remove them at the end of the search
	std::vector<PlannerState*> createdStates;
	std::vector<PlannerAction*> createdActions;
	
	//open and closed lists for the A* algorithm
	std::vector<PlannerState*> open;
	std::vector<PlannerState*> closed;

	original->gScore = 0.0f;
	
	//add the starting state to the open list
	open.push_back(original);

	//while the open list still contains states to search
	while (open.size() > 0)
	{
		//remember the state with the lowest 'F' score in the open list
		PlannerState* best = open[0];
		size_t bestI = 0;

		best->setHeuristic(goalState);

		//iterate through all states, finding the node with the smallest 'F' score
		for (size_t i = 1; i < open.size(); i++)
		{
			//store in a temp variable, saves performance and increases readability
			PlannerState* state = open[i];

			state->setHeuristic(goalState);

			//calculate the 'F' score of both nodes to compare them
			float bestF = best->gScore;// + best->hScore;
			float nodeF = state->gScore;// + state->hScore;

			//if the best 'F' score is higher than the current node's 'F' score
			if (bestF > nodeF)
			{
				//the current state is now the best state
				best = state;
				bestI = i;
			}
		}

		//the best state is the goal state
		if (best->hScore == 0.0f)
		{
			//list of actions to execute
			std::vector<PlannerAction*> actionList;

			PlannerState* current = best;

			//back-track to the root, remembering all actions on the way
			while (current->previous != nullptr)
			{
				//add the action to the front of the list
				actionList.insert(actionList.begin(), current->previousAction);

				//go to the previous state
				current = current->previous;
			}

			//remove all searched states
			for (size_t i = 0; i < createdStates.size(); i++)
			{
				//store in a temp variable, saves performance and increases readability
				PlannerState* state = createdStates[i];

				//don't delete the state if it is the goal state
				if (state != goalState)
				{
					delete createdStates[i];
				}
				else
				{
					state->gScore = INFINITY;
					state->previous = nullptr;
					state->previousAction = nullptr;
				}
			}

			//remove all searched actions
			for (size_t i = 0; i < createdActions.size(); i++)
			{
				//store in a temp variable, saves performance and increases readability
				PlannerAction* action = createdActions[i];

				//don't delete the action if it has been marked to be kept
				if (!action->keep)
				{
					delete action;
				}
			}

			return actionList;
		}

		//remove the state from the open list
		open.erase(open.begin() + bestI);

		//add the state to the closed list
		closed.push_back(best);

		//get the list of possible actions
		std::vector<PlannerAction*> actions = best->getActions();

		//iterate through all actions in the action list, generating a new state (search point) for each
		for (size_t i = 0; i < actions.size(); i++)
		{

			//store in a temp variable, saves performance and increases readability
			PlannerAction* action = actions[i];

			createdActions.push_back(action);

			//generate the new state
			PlannerState* state = action->execute(best);

			createdStates.push_back(state);

			//accumulate the cost
			state->gScore = best->gScore + action->cost;

			//give the state information about it's roots
			state->previous = best;
			state->previousAction = action;

			//add the state to the open list
			open.push_back(state);
		}
	}

	//the algorithm failed to find the ending point, all nodes connected to the starting state have been searched

	//remove all searched states
	for (size_t i = 0; i < createdStates.size(); i++)
	{
		delete createdStates[i];
	}

	//remove all searched actions
	for (size_t i = 0; i < createdActions.size(); i++)
	{
		delete createdActions[i];
	}

	return std::vector<PlannerAction*>();
}
