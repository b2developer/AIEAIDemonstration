#include "Blackboard.h"

//constructor, adds the initial ids to the list to be used by items
Blackboard::Blackboard(int maxItems)
{
	//loop through "maxItems" times, creating an entry
	for (int i = 0; i < maxItems; i++)
	{
		availableIds.push_back(i);
	}
}

//destructor, removes all remaining items
Blackboard::~Blackboard()
{
	//iterate through all items, removing each one
	for (size_t i = 0; i < items.size(); i++)
	{
		delete items[i];
	}
}

//gets called once per frame, ensures that items are deleted when they expire
void Blackboard::update(float deltaTime)
{
	//iterate through all items, ticking them closer to expiry
	for (size_t i = 0; i < items.size(); i++)
	{
		BlackboardItem* item = items[i];

		//deduct time from the timer
		item->timeRemaining -= deltaTime;
	
		//delete the item if it has expired
		if (item->timeRemaining <= 0)
		{
			//search through all ids, remove the id that matches the item
			for (std::vector<int>::iterator j = availableIds.begin(); j != availableIds.end(); j++)
			{
				//if the id matches, remove it
				if (*j == item->id)
				{
					availableIds.erase(j);
					break;
				}
			}

			delete item;
		}
	}
}
