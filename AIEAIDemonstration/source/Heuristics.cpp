#include "Heuristics.h"

//optimal for 4-way grids
float ManhattanDistance(Vector2 current, Vector2 end)
{
	return fabsf(end.x - current.x) + fabsf(end.y - current.y);
}

//optimal for 8-ways grids
float DiagonalDistance(Vector2 current, Vector2 end)
{
	//difference vector
	Vector2 d = end - current;
	d = Vector2(fabsf(d.x), fabsf(d.y));

	return SQRT2 * fminf(d.x, d.y) + (fmaxf(d.x, d.y) - fminf(d.x, d.y));
}

//optimal for any-direction graphs
float EuclideanDistance(Vector2 current, Vector2 end)
{
	//difference vector
	Vector2 d = end - current;

	return d.magnitude();
}