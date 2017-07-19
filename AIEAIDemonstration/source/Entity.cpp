#include "Entity.h"
#include "Transform.h"
#include "Application2D.h"

//called once per frame during the update call
void Entity::update()
{
	//add the velocity vector to the position vector from the transform
	transform->translation += velocity * appPtr->m_deltaTime;
}
