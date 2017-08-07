#include "Puzzle.h"

//constructor, creates an empty puzzle board
Puzzle::Puzzle(Vector2 _dimensions)
{
	dimensions = _dimensions;

	//iterate through the 2D array, assigning nullptr placeholders
	for (size_t i = 0; i < _dimensions.y; i++)
	{
		//add a new row
		pieces.push_back(std::vector<Piece*>());

		for (size_t j = 0; j < _dimensions.x; j++)
		{
			//add a new nullptr placeholder
			pieces[i].push_back(nullptr);
		}
	}
}



//copy constructor, duplicates pieces properly
Puzzle::Puzzle(Puzzle & other)
{
	dimensions = other.dimensions;
	freePosition = other.freePosition;

	//iterate through the 2D array, copying the two arrays
	for (size_t i = 0; i < other.pieces.size(); i++)
	{
		//add a new row
		pieces.push_back(std::vector<Piece*>());

		for (size_t j = 0; j < other.pieces[i].size(); j++)
		{
			//store in a temporary value for performance and readability
			Piece* piece = other.pieces[i][j];

			//add a free position (nullptr)
			if (piece == nullptr)
			{
				pieces[i].push_back(nullptr);
				continue;
			}

			//create a new piece to add to this puzzle's array
			Piece* copyPiece = new Piece();

			//copy the values to the new piece
			copyPiece->correctPosition = piece->correctPosition;
			copyPiece->currentPosition = piece->currentPosition;
			copyPiece->distance = piece->distance;

			//add the new piece
			pieces[i].push_back(copyPiece);
		}
	}

}



//destructor, removes all pieces
Puzzle::~Puzzle()
{
	//iterate through the 2D array, deleting all pieces
	for (size_t i = 0; i < pieces.size(); i++)
	{
		for (size_t j = 0; j < pieces[i].size(); j++)
		{
			//don't try and delete the free piece
			if (pieces[i][j] == nullptr)
			{
				continue;
			}

			//delete the piece
			delete pieces[i][j];
		}
	}
}



//sets the pieces in the correct positions
void Puzzle::setGoal()
{
	//set the bottom left corner as the free position
	freePosition = dimensions - Vector2(1, 1);

	//iterate through the 2D array, giving the positions and a random target position
	for (size_t i = 0; i < pieces.size(); i++)
	{
		for (size_t j = 0; j < pieces[i].size(); j++)
		{
			Vector2 target = Vector2((float)j, (float)i);

			//don't add a piece at the free position
			if (target == freePosition)
			{
				continue;
			}

			//create a new piece
			pieces[i][j] = new Piece();

			pieces[i][j]->correctPosition = Vector2((float)j, (float)i);
			pieces[i][j]->currentPosition = Vector2((float)j, (float)i);
		}
	}
}



//sets the pieces in random positions
void Puzzle::setRandom()
{
	//set the free position to a random position
	freePosition = Vector2((float)(rand() % (size_t)dimensions.x), (float)(rand() % (size_t)dimensions.y));

	//list of positions that haven't been used to be read from randomly
	std::vector<Vector2> positions;

	//iterate through the 2D array, giving a pair of coordinates in order
	for (size_t i = 0; i < dimensions.y; i++)
	{
		for (size_t j = 0; j < dimensions.x; j++)
		{
			Vector2 target = Vector2((float)j, (float)i);

			if (target != freePosition)
			{
				positions.push_back(target);
			}
		}
	}

	//iterate through the 2D array, giving the positions and a random target position
	for (size_t i = 0; i < pieces.size(); i++)
	{
		for (size_t j = 0; j < pieces[i].size(); j++)
		{
			Vector2 target = Vector2((float)j, (float)i);

			//don't add a piece at the free position
			if (target == freePosition)
			{
				continue;
			}

			pieces[i][j] = new Piece();

			pieces[i][j]->currentPosition = Vector2((float)j, (float)i);

			//choose a random vector from the list
			size_t chosenI = rand() % positions.size();

			pieces[i][j]->correctPosition = positions[chosenI];

			//erase the randomly chosen position from the list
			positions.erase(positions.begin() + chosenI);
		}
	}
}



//gets the incorrectness value of the puzzle
void Puzzle::setHeuristic(PlannerState * goal)
{
	//sum all of the manhattan distances
	float sum = 0.0f;

	//iterate through the 2D array, summing all incorrect positions
	for (size_t i = 0; i < pieces.size(); i++)
	{
		for (size_t j = 0; j < pieces[i].size(); j++)
		{
			//store in a temporary value for performance and readability
			Piece* piece = pieces[i][j];

			//don't test the free position as there isn't a piece there
			if (piece == nullptr)
			{
				continue;
			}

			//sum of the differences between the two axes
			float manhattanDistance = fabsf(piece->correctPosition.x - piece->currentPosition.x) + fabsf(piece->correctPosition.y - piece->currentPosition.y);

			piece->distance = manhattanDistance;

			sum += manhattanDistance;
		}
	}

	hScore = sum;
}



//get all actions that can be applied to the state
std::vector<PlannerAction*> Puzzle::getActions()
{
	//the possible movement positions
	Vector2 m1 = freePosition + Vector2(0, 1);
	Vector2 m2 = freePosition + Vector2(0, -1);
	Vector2 m3 = freePosition + Vector2(1, 0);
	Vector2 m4 = freePosition + Vector2(-1, 0);

	std::vector<Vector2> moves;

	//add all the movement positions to a list
	moves.push_back(m1);
	moves.push_back(m2);
	moves.push_back(m3);
	moves.push_back(m4);

	//list for valid actions to apply to the puzzle
	std::vector<PlannerAction*> actionList;

	//iterate through all positions in the move list, generating a new action for the valid positions
	for (size_t i = 0; i < moves.size(); i++)
	{
		//store in a temporary value for performance and readability
		Vector2 n = moves[i];

		//is the new position still in the array
		if (n.x >= 0 && n.x <= dimensions.x - 1 && n.y >= 0 && n.y <= dimensions.y - 1)
		{
			//create a new action
			PlannerAction* slide = new Slide();

			//define the action
			((Slide*)slide)->piecePosition = n;
			((Slide*)slide)->newPosition = freePosition;
			((Slide*)slide)->cost = 1.0f;

			//add the action to the list of actions
			actionList.push_back(slide);
		}
	}

	return actionList;
}



//applies the action to the game state
PlannerState* Slide::execute(PlannerState* original)
{
	//copy the puzzle by upcasting the planner state pointer to a puzzle and then de-referencing it
	PlannerState* newPuzzle = new Puzzle(*((Puzzle*)original));

	//upcast the puzzle
	Puzzle* puzzlePtr = (Puzzle*)newPuzzle;

	//new free position
	puzzlePtr->freePosition = piecePosition;

	//move the piece from the original position to the new position and set the original piece pointer to nullptr
	puzzlePtr->pieces[(size_t)newPosition.y][(size_t)newPosition.x] = puzzlePtr->pieces[(size_t)piecePosition.y][(size_t)piecePosition.x];
	puzzlePtr->pieces[(size_t)piecePosition.y][(size_t)piecePosition.x] = nullptr;

	//set the new position of the piece
	puzzlePtr->pieces[(size_t)newPosition.y][(size_t)newPosition.x]->currentPosition = newPosition;

	return newPuzzle;
}
