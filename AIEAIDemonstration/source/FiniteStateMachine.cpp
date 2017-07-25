#include "FiniteStateMachine.h"

//destructor, deletes all states and transitions
FiniteStateMachine::~FiniteStateMachine()
{
	//iterate through all vertices
	for (size_t i = 0; i < data.vertices.size(); i++)
	{
		Vertex<State*, Transition*>* vertex = data.vertices[i];

		//iterate through all edges in the vertex
		for (size_t j = 0; j < vertex->edges.size(); j++)
		{
			Edge<State*, Transition*> edge = vertex->edges[j];

			//delete the transition
			delete edge.value;
		}

		//delete the state
		delete vertex->data;
	}
}

//gets called once per frame (if the FSM is active)
void FiniteStateMachine::update(float deltaTime)
{
	//if the finite state machine doesn't have a current state, don't do anything
	if (currentVertex == nullptr)
	{
		return;
	}

	//update the state
	currentVertex->data->update(deltaTime);

	//iterate through all edges in the current vertex
	for (size_t i = 0; i < currentVertex->edges.size(); i++)
	{
		//pointer to the edge iterated to
		Edge<State*, Transition*>* edge = &currentVertex->edges[i];

		//get the transition from the edge
		Transition* transition = edge->value;

		//check what the transition's condition evaluates to
		if (transition->checkCondition())
		{
			//exit the current state
			currentVertex->data->onExit(deltaTime);

			//navigate to the next state
			currentVertex = edge->end;

			//enter the next state
			currentVertex->data->onEnter(deltaTime);

		}

	}
}

//links all states, transitions and conditions to the state machine
void FiniteStateMachine::setLinks()
{
	//iterate through all vertices in the graph
	for (size_t i = 0; i < data.vertices.size(); i++)
	{
		//link the vertex itself
		Vertex<State*, Transition*>* vertex = data.vertices[i];

		vertex->data->FSM = this;

		//iterate through all edges, connecting them to the FSM
		for (size_t j = 0; j < vertex->edges.size(); j++)
		{
			vertex->edges[j].value->FSM = this;
		}

	}
}
