#include "Circle.h"
#include "Transform.h"


//gets the minimum and maximnum dot products of the shape with the normal
Range Circle::project(Vector2 normal)
{
	//minimum and maximum points on the circle in the given direction
	Vector2 p1 = transform->translation - (normal * radius);
	Vector2 p2 = transform->translation + (normal * radius);

	//the corresponding dot products to the points
	float d1 = normal.dot(p1);
	float d2 = normal.dot(p2);

	//check which dot product is larger to order the returned range properly
	if (d2 > d1)
	{
		return Range( d1, d2 );
	}
	else
	{
		return Range( d2, d1 );
	}
}


//gets the normals to use from this shape
std::vector<Vector2> Circle::getNormals(Shape * other)
{
	//get the average coordinates of both shapes
	Range x = other->project(Vector2(1, 0));
	Range y = other->project(Vector2(0, 1));

	Vector2 otherMid = Vector2((x.min + x.max) / 2.0f, (y.min + y.max) / 2.0f);

	Vector2 relative = otherMid - transform->translation;

	//return a list with one normal in it
	return std::vector<Vector2>{ relative.normalised() };
}
