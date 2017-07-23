#pragma once
#include "BlackboardItem.h"
#include "BlackboardData.h"

#define TEMPLATE template<typename T>

/*
* class Blackboard
*
* a central point of access to information and commuication
* for agents, prevents coupling and enables systems to be
* almost seperate from each other
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class Blackboard
{
public:

	//container of all available ids
	std::vector<int> availableIds;

	//container of all items on the blackboard
	std::vector<BlackboardItem*> items;

	/*
	* Blackboard()
	* constructor, adds the initial id to the list to be used
	*
	* @param int maxItems - the maximum amount of items that can be in this blackboard
	*/
	Blackboard(int maxItems);


	/*
	* ~Blackboard()
	* destructor, removes all remaining items
	*/
	~Blackboard();


	/*
	* update
	*
	* updates the timer of all items on the board
	* deletes the item if they expire
	*
	* @param float deltaTime - the amount of time passed since the last frame
	* @returns void
	*/
	void update(float deltaTime);

	
	/*
	* registerItem
	* template function with two types
	*
	* adds an item of type T and returns
	* the id of the item in the blackboard
	*
	* @param float lifeTime - how long in seconds the item remains active
	* @returns int - the id of the item
	*/
	TEMPLATE2
	int registerItem(float lifeTime)
	{
		BlackboardData<T, U>* item = new BlackboardData<T, U>();

		//give the time remaining to the item
		item->timeRemaining = lifeTime;

		//create the new object (the data)
		T* data = new T();

		item->data = data;
		item->id = availableIds.end();

		availableIds.pop_back();
	}


	/*
	* searchFor
	* template function with two types
	*
	* searches for an item that matches
	* the given id, returns it if it is found
	* returns nullptr if it wasn't
	*
	* @param int id - the id to search for
	* @returns BlackboardData<T, U>* - pointer to the item or nullptr
	a
	*/
	TEMPLATE2
	BlackboardData<T, U>* searchFor(int id)
	{
		//iterate through all items, comparing their ids
		for (size_t i = 0; i < items.size(); i++)
		{
			BlackboardItem* item = items[i];

			//if the item's id matches, return it
			if (id == item->id)
			{
				//downcast the item
				return (BlackboardData<T, U>*)item;
			}
		}

		//failed to find the item, return nullptr
		return nullptr;
	}
};