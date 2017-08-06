#include "Circle.h"
#include "Transform.h"


//gets the minimum and maximnum dot products of the shape with the normal
Range Circle::project(Vector2 normal)
{
	Matrix2 scaleMat = Matrix2(transform->scale.x, 0, 0, transform->scale.y);

	//minimum and maximum points on the circle in the given direction
	Vector2 p1 = transform->translation - (normal * radius) * scaleMat;
	Vector2 p2 = transform->translation + (normal * radius) * scaleMat;

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
	Vector2 relative = other->transform->translation - transform->translation;

	//return a list with one normal in it
	return std::vector<Vector2>{ relative.normalised() };
}
