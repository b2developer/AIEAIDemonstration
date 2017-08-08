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
			copyPiece->renderPosition = piece->renderPosition;
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
	//maximum difference to be considered equal
	float epsilon = 0.01f;

	//set the bottom left corner as the free position
	freePosition = dimensions - Vector2(1, 1);

	//iterate through the 2D array, giving the positions and a random target position
	for (size_t i = 0; i < pieces.size(); i++)
	{
		for (size_t j = 0; j < pieces[i].size(); j++)
		{
			Vector2 target = Vector2((float)j, (float)i);

			//don't add a piece at the free position
			if ((target - freePosition).sqrMagnitude() <= epsilon)
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
void Puzzle::setRandom(size_t iterations)
{

	//cache all directions
	std::vector<Vector2> directions;

	directions.push_back(Vector2(0, 1));
	directions.push_back(Vector2(0, -1));
	directions.push_back(Vector2(1, 0));
	directions.push_back(Vector2(-1, 0));


	size_t prevRandNum = 1;
	size_t prevRandNumOpp = 1;

	//apply 'n' random moves
	for (size_t i = 0; i < iterations; i++)
	{
		std::vector<Vector2> directionsCopy = directions;

		directionsCopy.erase(directionsCopy.begin() + prevRandNumOpp);

		//create a random move
		PlannerAction* slide = new Slide();

		//choose a random direction
		size_t randNum = rand() % directionsCopy.size();

		//initial search direction and position
		Vector2 offset = directionsCopy[randNum];;
		Vector2 piecePosition = freePosition + offset;

		//is the tile inside the puzzle's dimensions
		while (!(piecePosition.x >= 0 && piecePosition.y >= 0 && piecePosition.x <= dimensions.x - 1 && piecePosition.y <= dimensions.y - 1))
		{
			randNum = rand() % directionsCopy.size();

			//search direction
			offset = directionsCopy[randNum];

			//position of the tile to move
			piecePosition = freePosition + offset;

		} 

		//give the action it's data and then execute it
		((Slide*)slide)->newPosition = freePosition;
		((Slide*)slide)->piecePosition = piecePosition;

		((Slide*)slide)->executeDirect(this);

		prevRandNum = randNum;

		//make prevRandNumOpp the inverse of prevRandNum
		switch (prevRandNum)
		{
		case 0: prevRandNumOpp = 1; break;
		case 1: prevRandNumOpp = 0; break;
		case 2: prevRandNumOpp = 3; break;
		case 3: prevRandNumOpp = 2; break;
		}


		delete slide;
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



//checks if the given action is the opposite of this action
bool Slide::isReverse(PlannerAction * other)
{
	//maximum difference to be considered equal
	float epsilon = 0.01f;

	//upcast the other planner action
	Slide* otherSlide = (Slide*)other;

	//compare the opposite points of the swaps
	bool c1 = (piecePosition - otherSlide->newPosition).sqrMagnitude() < epsilon;
	bool c2 = (otherSlide->piecePosition - newPosition).sqrMagnitude() < epsilon;

	//only return true if both tests are successful
	return c1 && c2;
}



//applies the action to the game state without copying it
void Slide::executeDirect(PlannerState * original)
{
	//upcast the puzzle
	Puzzle* puzzlePtr = (Puzzle*)original;

	//new free position
	puzzlePtr->freePosition = piecePosition;

	//move the piece from the original position to the new position and set the original piece pointer to nullptr
	puzzlePtr->pieces[(size_t)newPosition.y][(size_t)newPosition.x] = puzzlePtr->pieces[(size_t)piecePosition.y][(size_t)piecePosition.x];
	puzzlePtr->pieces[(size_t)piecePosition.y][(size_t)piecePosition.x] = nullptr;

	//set the new position of the piece
	puzzlePtr->pieces[(size_t)newPosition.y][(size_t)newPosition.x]->currentPosition = newPosition;
}
