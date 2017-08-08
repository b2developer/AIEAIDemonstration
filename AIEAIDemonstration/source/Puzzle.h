#pragma once
#include <vector>
#include "MathsLibrary.h"
#include "Planner.h"


/*
* class Piece
*
* part of a puzzle, many other pieces
* are contained by a puzzle also
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Piece
{

public:

	//distance to the correct position
	float distance = 0.0f;

	//current position of the piece
	Vector2 currentPosition = Vector2(0, 0);

	//the desired position of the piece
	Vector2 correctPosition = Vector2(0, 0);

	//position to render at relative to the other pieces, gets interpolated by a renderer
	Vector2 renderPosition = Vector2(0, 0);

	/*
	* Piece()
	* default constructor
	*/
	Piece() {}


	/*
	* ~Piece()
	* default destructor
	*/
	~Piece() {}
};



/*
* class Puzzle
* child class of PlannerState
*
* a planner state that is a 2D grid of pieces that may
* or may not be arranged correctly
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Puzzle : public PlannerState
{

public:
	
	//the dimensions of the 2D grid
	Vector2 dimensions = Vector2(0, 0);

	//there has to be one position that doesn't contain a piece
	Vector2 freePosition = Vector2(0, 0);

	//2D array of pieces
	std::vector<std::vector<Piece*>> pieces;

	/*
	* Puzzle()
	* constructor, assigns the dimensions and creates new pieces
	* @param Vector2 _dimensions - the input dimensions
	*/
	Puzzle(Vector2 _dimensions);


	/*
	* Puzzle(Puzzle& other)
	* copy constructor, creates new piece objects with the same positions
	* @param Puzzle& other - reference to the puzzle to copy from
	*/
	Puzzle(Puzzle& other);


	/*
	* ~Puzzle()
	* destructor, deletes all pieces
	*/
	~Puzzle();


	/*
	* setGoal
	*
	* sets the puzzle with pieces that
	* are placed correctly
	*
	* @returns void
	*/
	void setGoal();


	/*
	* setRandom
	*
	* sets the puzzle with pieces that
	* are placed randomly
	*
	* @param size_t iterations - the amount of random moves to attempt
	* @returns void
	*/
	void setRandom(size_t iterations);


	/*
	* setHeuristic
	* overrides PlannerState's setHeuristic(PlannerState* goal)
	*
	* gets the 'H' score of the state by
	* comparing itself to the goal state
	*
	* @param PlannerState* goal - the goal state to compare to
	* @returns void
	*/
	void setHeuristic(PlannerState* goal) override;


	/*
	* getActions
	* overrides PlannerState's getActions()
	*
	* gets a list of actions that can be applied
	* to the state and generate a different state
	*
	* @returns std::vector<PlannerAction*> - the list of actions that can be applied to this state
	*/
	std::vector<PlannerAction*> getActions() override;
};



/*
* class Slide
* child class of PlannerAction
*
* a planner action that slides a piece of a puzzle
* to a new location, moving the new piece
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Slide : public PlannerAction
{

public:

	//the position of the piece and the new position to move it to
	Vector2 piecePosition = Vector2(0, 0);
	Vector2 newPosition = Vector2(0, 0);

	/*
	* Slide()
	* default constructor
	*/
	Slide() {}


	/*
	* ~Slide()
	* default destructor
	*/
	~Slide() {};


	/*
	* execute
	* overrides PlannerAction's execute()
	*
	* applies the action to the state
	* and returns it as a new state
	*
	* @param PlannerState* - the state to apply the action to
	* @returns PlannerState* - the new state with the action applied to it
	*/
	PlannerState* execute(PlannerState* original) override;


	/*
	* isReverse
	* overrides PlannerAction's isReverse()
	*
	* checks if this action applies the opposite
	* effect of the other action
	*
	* @param PlannerAction* other - pointer to the other action
	* @returns bool - indicates if 'other' is the reverse action
	*/
	bool isReverse(PlannerAction* other) override;


	/*
	* executeDirect
	*
	* applies the action directly to the state that called it
	*
	* @param PlannerState* - the state to apply the action to
	* @returns void
	*/
	void executeDirect(PlannerState* original);

};