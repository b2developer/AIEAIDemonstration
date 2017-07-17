#include "Director.h"

//allocates a new gameobject and populates it with components from the spawner
GameObject * Director::createGameObject()
{
	//allocate the gameobject
	GameObject* creation = new GameObject();

	//add components from the spawner
	employee->addComponents(creation);

	return creation;
}
