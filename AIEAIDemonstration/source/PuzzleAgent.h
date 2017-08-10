#pragma once
#include "Agent.h"
#include <vector>
#include "MathsLibrary.h"
#include "Planner.h"
#include "Puzzle.h"

//forward declarations
class Transform;
class Application2D;



/*
* class PuzzleAgent
* child class of Agent
*
* a component that solves sliding tile puzzles
* and demonstrates these solutions with an animation
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class PuzzleAgent : public Agent
{
public:

	//link to the planner that solves the puzzles
	Planner* planner = nullptr;

	//dimensions of the puzzle
	int puzzleISize = 4;
	int puzzleJSize = 4;

	//amount of iterations the agent makes when randomly generating puzzles to solve
	size_t randomIterations = 25;

	//puzzle that the agent is trying to solve
	Puzzle* current = nullptr;

	//list of actions to execute on the puzzle
	std::vector<PlannerAction*> plannedActions;

	//timer to delay solving
	float presentationTimer = 0.0f;

	//amount of time to stop the agent from solving in order to present the puzzle
	float presentationDuration = 3.0f;

	//timer to delay steps
	float stepTimer = 0.0f;

	//how long a step takes to complete
	float stepDuration = 0.5f;

	/*
	* PuzzleAgent()
	* default constructor
	*/
	PuzzleAgent() {}


	/*
	* ~PuzzleAgent()
	* destructor, deletes the planner, current state and all planned moves
	*/
	~PuzzleAgent();


	/*
	* update
	* abstract function
	* overrides Agent's update()
	*
	* gets called once per frame, updates the state
	* of the trading agent
	*
	* @returns void
	*/
	void update() override;

};