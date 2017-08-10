#pragma once
#include <vector>
#include "Component.h"

/*
* class GameObject
*
* groups components together to identify
* them as part of a common object
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class GameObject
{
public:

	//list of components held by this
	std::vector<Component*> components;

	//counts how many groups this gameobject is in
	int groups = 0;

	/*
	* GameObject
	* default constructor
	*/
	GameObject() {}


	/*
	* ~GameObject
	* destructor, terminates all components
	*/
	~GameObject();


	/*
	* initialse
	*
	* called when the game-object gets created 
	*
	* @returns void
	*/
	void initialise();


	/*
	* update
	*
	* called once per frame while the gameobject is active
	*
	* @returns void
	*/
	void update();


	/*
	* draw
	*
	* called once per frame after the update, calls upon
	* all renderer components stored to render themselves
	*
	* @returns void
	*/
	void draw();


	/*
	* GetComponentOfType
	* template function
	*
	* searches through the list of components in this
	* and returns the first component which matches the type
	* returns nullptr otherwise
	*
	* @returns T* - either nullptr or a pointer to a component
	*/
	template<typename T>
	T* GetComponentOfType()
	{
		//iterate through all components
		for (size_t i = 0; i < components.size(); i++)
		{
			//store the component temporarily to avoid performance loss
			Component* component = components[i];

			//if the dynamic cast succeeds on the template type, the component type matches the input type
			if (dynamic_cast<T*>(component) != nullptr)
			{
				return component;
			}
		}
	}


	/*
	* GetAllComponentsOfType
	* template function
	*
	* searches through the list of components in this
	* and returns a list of all components that match
	* a specific type
	*
	* @returns std::vector<T*> - a list of all components that match the type
	*/
	template<typename T>
	std::vector<T*> GetAllComponentsOfType()
	{
		std::vector<T*> foundComponents;

		//iterate through all components
		for (size_t i = 0; i < components.size(); i++)
		{
			//store the component temporarily to avoid performance loss
			Component* component = components[i];

			//if the dynamic cast succeeds on the template type, the component type matches the input type
			if (dynamic_cast<T*>(component) != nullptr)
			{
				//add the component to the list
				foundComponents.push_back(component);
			}
		}

		return foundComponents;
	}
	
};

