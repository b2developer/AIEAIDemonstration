#include "GameObject.h"
#include "Renderer.h"

//destructor, terminates all components
GameObject::~GameObject()
{
	//iterate through all components, deleting each one
	for (size_t i = 0; i < components.size(); i++)
	{
		delete components[i];
	}
}

//called when the object gets created
void GameObject::initialise()
{
	//iterate through all components, initialising them
	for (size_t i = 0; i < components.size(); i++)
	{
		//store the component temporarily to avoid performance loss
		Component* component = components[i];

		component->initialise();
	}
}

//called once per frame, updates all components (except renderers)
void GameObject::update()
{
	//iterate through all components, updating them if they aren't renderers
	for (size_t i = 0; i < components.size(); i++)
	{
		//store the component temporarily to avoid performance loss
		Component* component = components[i];

		//if the dynamic cast fails, the component isn't a renderer
		if (dynamic_cast<Renderer*>(component) == nullptr)
		{
			component->update();
		}
	}
}

//called after the update call, updates all renderers
void GameObject::draw()
{
	//iterate through all components, updating them only if they are renderers
	for (size_t i = 0; i < components.size(); i++)
	{
		//store the component temporarily to avoid performance loss
		Component* component = components[i];

		//if the dynamic cast succeeds, the component must be a renderer
		if (dynamic_cast<Renderer*>(component) != nullptr)
		{
			component->update();
		}
	}
}
