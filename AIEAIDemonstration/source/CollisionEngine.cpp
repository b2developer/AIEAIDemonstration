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

//calculates which side of a line formed by A and B P is on
int CollisionEngine::calculateSide(Vector2 A, Vector2 B, Vector2 P)
{
	//make A the origin
	Vector2 OB = B - A;
	Vector2 OP = P - A;

	//get the direction of the line
	Vector2 BA = OB.normalised();

	//get the normal of the line
	Vector2 BAn = BA.normal();

	//project the point onto the line
	Vector2 proj = BA * BA.dot(OP);

	Vector2 Pn = (OP - proj).normalised();

	//get the relative direction
	float dot = BAn.dot(Pn);

	//if the normals point in the same direction
	if (dot > 0.5f)
	{
		return 1;
	}
	//if the normals point in the opposite direction
	else if (dot < -0.5f)
	{
		return -1;
	}
	
	//the dot is too small, indicating that one of the vectors is null
	return 0;
}

//checks for a collision between two line segments
bool CollisionEngine::lineCollisionCheck(Vector2 A, Vector2 B, Vector2 C, Vector2 D, float tolerance)
{
	//list to store normals for a seperating axis test
	std::vector<Vector2> normals;

	//get the directional vectors of both lines
	Vector2 BA = (B - A).normalised();
	Vector2 DC = (D - C).normalised();

	//add the directional vectors and their normals to the list of vectors to project
	normals.push_back(BA);
	normals.push_back(DC);
	normals.push_back(BA.normal());
	normals.push_back(DC.normal());

	//iterate through all normals
	for (size_t i = 0; i < normals.size(); i++)
	{
		//store in a temp variable for performance and readability
		Vector2 normal = normals[i];

		//project all of the points
		float min1 = normal.dot(A);
		float max1 = normal.dot(B);
		float min2 = normal.dot(C);
		float max2 = normal.dot(D);

		//check that the min dots are smaller than the max dots, otherwise swap them
		if (min1 > max1)
		{
			float temp = min1;
			min1 = max1;
			max1 = temp;
		}

		if (min2 > max2)
		{
			float temp = min2;
			min2 = max2;
			max2 = temp;
		}

		//no-overlap test
		if (max1 <= min2 + tolerance || min1 + tolerance >= max2)
		{
			return false;
		}

	}

	//no seperating axis was found, the lines are intersecting
	return true;
}
