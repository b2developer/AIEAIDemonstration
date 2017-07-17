#include "FiniteStateMachine.h"

//gets called once per frame (if the FSM is active)
void FiniteStateMachine::update(float deltaTime)
{
	//update the state
	currentVertex->data->update(deltaTime);

	//iterate through all edges in the current vertex
	for (size_t i = 0; i < currentVertex->edges.size; i++)
	{
		//pointer to the edge iterated to
		Edge<State*, Transition*>* edge = &currentVertex->edges[i];

		//get the transition from the edge
		Transition* transition = edge->value;

		//check what the transition's condition evaluates to
		if (transition->checkCondition())
		{
			//navigate to the next state
			currentVertex = edge->end;
			break;
		}

	}
}
