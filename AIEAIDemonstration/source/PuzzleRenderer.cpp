#include "PuzzleRenderer.h"
#include "Application2D.h"
#include "Transform.h"

#include "Box.h"
#include "Puzzle.h"

//called once per frame during the draw call
void PuzzleRenderer::update()
{
	//iterate through all pieces in the 2D array, checking for incorrect render positions
	for (size_t i = 0; i < puzzle->pieces.size(); i++)
	{
		for (size_t j = 0; j < puzzle->pieces[i].size(); j++)
		{
			//store in a temporary value for performance and readability
			Piece* piece = puzzle->pieces[i][j];

			//don't render a piece that doesn't exist
			if (piece == nullptr)
			{
				continue;
			}

			//relative position from the rendered piece to the current position
			Vector2 relative = piece->currentPosition - piece->renderPosition;

			//move the piece towards the correct position if it is out of position
			if (relative.magnitude() < movementSpeed * appPtr->m_deltaTime)
			{
				piece->renderPosition = piece->currentPosition;
			}
			else
			{
				piece->renderPosition += relative.normalised() * movementSpeed * appPtr->m_deltaTime;
			}

			//bootstrap renders from the bottom, UVs start from the top
			size_t invertedI = (size_t)puzzle->dimensions.y - i;

			//get the size of a single piece of the puzzle
			Vector2 boxOffset = Vector2((box->max.x - box->min.x) * piece->renderPosition.x / puzzle->dimensions.x, (box->max.y - box->min.y) * (puzzle->dimensions.y - piece->renderPosition.y) / puzzle->dimensions.y);
			Vector2 pieceSize = Vector2((box->max.x - box->min.x) / puzzle->dimensions.x, (box->max.y - box->min.y) / puzzle->dimensions.y);

			//UV coordinates of the texture
			Vector2 UV1 = Vector2((float)piece->correctPosition.x / puzzle->dimensions.x, (float)piece->correctPosition.y / puzzle->dimensions.y);
			Vector2 UV2 = Vector2((float)(piece->correctPosition.x + 1) / puzzle->dimensions.x, (float)(piece->correctPosition.y + 1) / puzzle->dimensions.y);

			//set the texture coordinates
			appPtr->m_renderer2D->setUVRect(UV1.x, UV1.y, UV2.x - UV1.x, UV2.y - UV1.y);

			appPtr->m_renderer2D->drawSprite(texture, transform->translation.x + boxOffset.x, transform->translation.y + boxOffset.y, pieceSize.x, pieceSize.y);
		}
	}

	//reset the UV rect after rendering
	appPtr->m_renderer2D->setUVRect(0, 0, 1, 1);
}
