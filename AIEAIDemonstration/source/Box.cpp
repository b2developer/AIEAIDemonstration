#include "Box.h"
#include "Transform.h"


//gets the minimum and maximnum dot products of the shape with the normal
Range Box::project(Vector2 normal)
{
	Matrix3 transMat = transform->getMatrix();

	/*
	* arrangement of the corners below
	*
	* 2 - - - - - 3
	* |		 (max)|
	* |			  |
	* |			  |
	* |(min) 	  |
	* 1 - - - - - 4
	*/
	Vector2 p1 = min;
	Vector2 p2 = Vector2(min.x, max.y);
	Vector2 p3 = max;
	Vector2 p4 = Vector2(max.x, min.y);

	//transform the 4 corners
	p1 = (Vector3)p1 * transMat;
	p2 = (Vector3)p2 * transMat;
	p3 = (Vector3)p3 * transMat;
	p4 = (Vector3)p4 * transMat;

	//dot product all of the corners
	std::vector<float> dots = std::vector<float>{ normal.dot(p1), normal.dot(p2), normal.dot(p3), normal.dot(p4) };

	//the minimum and maximum dot products
	float minDot = MAX;
	float maxDot = MIN;

	//always 4 corners in a box
	for (int i = 0; i < 4; i++)
	{
		float dot = dots[i];

		//if the dot is the smallest dot product
		if (dot < minDot)
		{
			minDot = dot;
		}

		//if the dot is the biggest dot product
		if (dot > maxDot)
		{
			maxDot = dot;
		}
	}

	return Range(minDot, maxDot);
}


//gets the normals to use from this shape
std::vector<Vector2> Box::getNormals(Shape * other)
{
	Matrix3 transMat = transform->getMatrix();

	/*
	* arrangement of the corners below
	*
	* 2 - - - - - 3
	* |		 (max)|
	* |			  |
	* |			  |
	* |(min) 	  |
	* 1 - - - - - 4
	*/
	Vector2 p1 = min;
	Vector2 p2 = Vector2(min.x, max.y);

	//transform the 4 corners
	p1 = (Vector3)p1 * transMat;
	p2 = (Vector3)p2 * transMat;
	
	//get the normals
	Vector2 n2 = p2 - p1;

	//one normal is enough to create the rest using its normals and negation
	n2.normalise();

	Vector2 n4 = n2 * -1.0f;
	Vector2 n3 = n2.normal();
	Vector2 n1 = n3 * -1.0f;

	//return a list of the 4 normals
	return std::vector<Vector2>{n1, n2, n3, n4};
}