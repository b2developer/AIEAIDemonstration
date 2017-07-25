#pragma once
#include <vector>

#define TEMPLATE template<typename T>

//forward declaration
class BlackboardReference;

/*
* class BlackboardItem
*
* base object for an item that agents can 
* observe through the blackboard it is stored in
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class BlackboardItem
{
public:

	//number that identifies the item
	int id = 0;

	//the amount of seconds remaining in this blackboard item's lifetime
	float timeRemaining = 0.0f;

	//can the item be set to countdown
	bool timeLocked = false;

	//list of references to this item
	std::vector<BlackboardReference*> references;

	/*
	* BlackboardItem()
	* default constructor
	*/
	BlackboardItem() {}


	/*
	* ~BlackboardItem()
	* virtual function
	* destructor, tells all references that the item no longer exists
	*/
	virtual ~BlackboardItem();


};