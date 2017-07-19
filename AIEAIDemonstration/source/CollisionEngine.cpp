#include "CollisionEngine.h"

//singleton creation/aquisition
CollisionEngine * CollisionEngine::getInstance()
{
	static CollisionEngine* singleton = nullptr;

	//if the singleton hasn't been created yet, create it
	if (singleton == nullptr)
	{
		singleton = new CollisionEngine();
	}

	return singleton;
}

//returns if the shapes are colliding or not
bool CollisionEngine::simpleCollisionCheck(Shape * A, Shape * B)
{
	//get the normals from each shape
	std::vector<Vector2> aNormals = A->getNormals(B);
	std::vector<Vector2> bNormals = B->getNormals(A);

	//combine the arrays
	std::vector<Vector2> normals = aNormals;
	aNormals.insert(aNormals.end(), bNormals.begin(), bNormals.end());

	//iterate through all normals
	for (size_t i = 0; i < normals.size(); i++)
	{
		Vector2 normal = normals[i];

		//get the projections of both shapes
		Range aRange = A->project(normal);
		Range bRange = B->project(normal);

		//no-overlap test
		if (aRange.max <= bRange.min || aRange.min >= bRange.max)
		{
			return false;
		}
	}

	return true;
}

//returns detailed information about the collision (if one occurs)
AdvancedCollision CollisionEngine::advancedCollisionCheck(Shape * A, Shape * B)
{
	//get the normals from each shape
	std::vector<Vector2> aNormals = A->getNormals(B);
	std::vector<Vector2> bNormals = B->getNormals(A);

	//combine the arrays
	std::vector<Vector2> normals = aNormals;
	aNormals.insert(aNormals.end(), bNormals.begin(), bNormals.end());

	float bestDistance = MAX;
	Vector2 bestSeperation = Vector2(0, 0);

	//iterate through all normals
	for (size_t i = 0; i < normals.size(); i++)
	{
		Vector2 normal = normals[i];

		//get the projections of both shapes
		Range aRange = A->project(normal);
		Range bRange = B->project(normal);

		//no-overlap test
		if (aRange.max <= bRange.min || aRange.min >= bRange.max)
		{
			//return a failed collision test
			AdvancedCollision fail = AdvancedCollision();

			fail.colliding = false;
			fail.A = A;
			fail.B = B;

			return fail;
		}
		else
		{
			//an intersection along this axis exists, get seperation data

			//test if the min/max of 'A' are in the range of 'B'
			bool minIn = aRange.min < bRange.max;
			bool maxIn = aRange.max > bRange.min;

			float distance = 0.0f;
			Vector2 seperation = Vector2(0, 0);

			if (minIn && maxIn) //both points intersect the range
			{
				float d1 = aRange.min - bRange.max;
				float d2 = aRange.max - bRange.min;

				distance = d2;
				seperation = normal * d2;

				//test which seperation vector is bigger
				if (d2 > d1)
				{
					distance = d1;
					seperation = normal * d1;
				}
			}
			else if (minIn) //only the min point intersects
			{
				distance = aRange.min - bRange.max;
				seperation = normal * distance;
			}
			else //only the max point intersects
			{
				distance = aRange.max - bRange.min;
				seperation = normal * distance;
			}

			//get the absolute value of the distance
			float absDist = distance < 0 ? -distance : distance;

			//the best seperation is the smallest, check if the smallest distance found so far is larger than the distance just calculated
			if (absDist < bestDistance)
			{
				bestDistance = absDist;
				bestSeperation = seperation;
			}

		}
	}

	//return a successful collision test
	AdvancedCollision success = AdvancedCollision();

	success.colliding = true;
	success.A = A;
	success.B = B;
	success.MTV = bestSeperation;

	return AdvancedCollision();
}
