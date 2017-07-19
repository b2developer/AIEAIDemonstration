#include "Polygon.h"
#include "Transform.h"


//gets the minimum and maximnum dot products of the shape with the normal
Range Polygon::project(Vector2 normal)
{
	Matrix3 transMat = transform->getMatrix();

	//storage for the transformed points
	std::vector<Vector2> transformedPoints = std::vector<Vector2>();

	//iterate through all points, apply a transformation to a copy
	for (size_t i = 0; i < points.size(); i++)
	{
		transformedPoints.push_back((Vector3)points[i] * transMat);
	}

	//storage for the dots
	std::vector<float> dots = std::vector<float>();

	//iterate through all transformed points, getting the dot product with the normal of each one
	for (size_t i = 0; i < transformedPoints.size(); i++)
	{
		dots.push_back(normal.dot(points[i]));
	}

	//the minimum and maximum dot products
	float minDot = MAX;
	float maxDot = MIN;

	//always 4 corners in a box
	for (size_t i = 0; i < dots.size(); i++)
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
std::vector<Vector2> Polygon::getNormals(Shape * other)
{
	Matrix3 transMat = transform->getMatrix();

	//storage for the transformed points
	std::vector<Vector2> transformedPoints = std::vector<Vector2>();

	//iterate through all points, apply a transformation to a copy 
	for (size_t i = 0; i < points.size(); i++)
	{
		transformedPoints.push_back((Vector3)points[i] * transMat);
	}

	//storage for the normals
	std::vector<Vector2> normals = std::vector<Vector2>();

	//iterate through all transformed points, get the normal from each
	for (size_t i = 0; i < transformedPoints.size(); i++)
	{
		//get the normalised vector from this point to the next (the next point for the last point in the points array is the first point)
		Vector2 size = transformedPoints[i + 1 % transformedPoints.size()] - transformedPoints[i];
		size.normalise();

		//get the normal of the side vector
		Vector2 normal = size.normal();

		normals.push_back(normal);
	}

	return normals;
}