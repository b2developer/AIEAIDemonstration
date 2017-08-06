#include "PrimitiveRenderer.h"
#include "Application2D.h"
#include "Transform.h"

#include "Circle.h"
#include "Box.h"
#include "Polygon.h"

//called once per frame during the draw call
void PrimitiveRenderer::update()
{
	//don't render a shape that doesn't exist
	if (shape == nullptr)
	{
		return;
	}

	//determine which shape it is, the shape that it won't be a nullptr
	Circle* circle = dynamic_cast<Circle*>(shape);
	Box* box = dynamic_cast<Box*>(shape);
	Polygon* polygon = dynamic_cast<Polygon*>(shape);

	//set the rendering colour specfic to this primitive
	appPtr->m_renderer2D->setRenderColour(r, g, b);

	if (circle != nullptr)
	{
		appPtr->m_renderer2D->drawCircle(transform->translation.x, transform->translation.y, circle->radius, depth);
	}
	else if (box != nullptr)
	{
		//half-extents on the box
		Vector2 halfExtents = (box->max - box->min) * 0.5f;

		//scale the half-extents
		halfExtents.x *= transform->scale.x;
		halfExtents.y *= transform->scale.y;

		appPtr->m_renderer2D->drawBox(transform->translation.x, transform->translation.y, halfExtents.x * 2.0f, halfExtents.y * 2.0f, transform->rotation, depth);
	}
	else if (polygon != nullptr)
	{
		std::vector<Vector2> points = polygon->points;

		//iterate through the points, transforming them
		for (size_t i = 0; i < points.size(); i++)
		{
			points[i].x *= transform->scale.x;
			points[i].y *= transform->scale.y;

			//rotation matrix that matches the transformation from the transform component
			Matrix2 rotationMatrix;
			rotationMatrix.identity();
			rotationMatrix.setRotate(transform->rotation);

			//apply a rotation to the point
			points[i] = points[i] * rotationMatrix;

			//apply a scale to the point
			points[i].x *= transform->scale.x;
			points[i].y *= transform->scale.y;

			//apply a translation to the point
			points[i] += transform->translation;
		}

		//store the sum, then divide it to get the average point
		Vector2 midpoint = Vector2(0, 0);

		//iterate through all points, summing them
		for (size_t i = 0; i < points.size(); i++)
		{
			midpoint += points[i];
		}

		midpoint = midpoint / (float)points.size();

		//iterate through all points, drawing a triangle for each
		for (size_t i = 0; i < points.size(); i++)
		{
			//get the first two points involved in the triangle
			size_t first = i;
			size_t second = (i+1) % points.size();

			appPtr->m_renderer2D->drawTriangle(points[second].x, points[second].y, points[first].x, points[first].y, midpoint.x, midpoint.y, depth);
		}
	}

	//reset the rendering colour
	appPtr->m_renderer2D->setRenderColour(1.0f, 1.0f, 1.0f);
	
}
