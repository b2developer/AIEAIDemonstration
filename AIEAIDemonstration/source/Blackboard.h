#pragma once
#include "BlackboardItem.h"
#include "BlackboardData.h"
#include "BlackboardReference.h"

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
	* adds an item of type T and returns the
	* raw item data, this is used when the other
	* system wants to set the data but wont be using
	* the pointer for more than one frame
	*
	* @param float lifeTime - how long in seconds the item remains active
	* @returns BlackboardData<T, U>* - pointer to the item
	*/
	TEMPLATE2
	BlackboardData<T, U>* registerItem(float lifeTime)
	{
		BlackboardData<T, U>* item = new BlackboardData<T, U>();

		//give the time remaining to the item
		item->timeRemaining = lifeTime;

		//get the last id and pop it from the list
		item->id = *(availableIds.end() - 1);

		availableIds.pop_back();

		//add the item to the list
		items.push_back(item);

		return item;
	}

	
	/*
	* registerItemWithReference
	* template function with two types
	*
	* adds an item of type T and returns
	* a blackboard reference to the item
	*
	* @param float lifeTime - how long in seconds the item remains active
	* @returns BlackboardReference* - an item that other systems can use to access the data safely
	*/
	TEMPLATE2
	BlackboardReference* registerItemWithReference(float lifeTime)
	{
		BlackboardData<T, U>* item = new BlackboardData<T, U>();

		//give the time remaining to the item
		item->timeRemaining = lifeTime;

		//get the last id and pop it from the list
		item->id = *(availableIds.end() - 1);

		availableIds.pop_back();

		//add the item to the list
		items.push_back(item);

		//set-up the new blackboard reference
		BlackboardReference* bref = new BlackboardReference();

		bref->item = item;
		bref->exists = true;

		//add the blackboard reference to the item
		item->references.push_back(bref);

		return bref;
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
	* @returns BlackboardReference* - pointer to the item or nullptr
	a
	*/
	TEMPLATE2
	BlackboardReference* searchFor(int id)
	{
		//iterate through all items, comparing their ids
		for (size_t i = 0; i < items.size(); i++)
		{
			BlackboardItem* item = items[i];

			//if the item's id matches, return it
			if (id == item->id)
			{
				//set-up the new blackboard reference
				BlackboardReference* bref = new BlackboardReference();

				bref->item = item;
				bref->exists = true;

				return bref;
			}
		}

		//failed to find the item, return nullptr
		return nullptr;
	}
};