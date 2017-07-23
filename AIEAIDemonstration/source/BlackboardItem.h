#pragma once
#include <vector>

#define TEMPLATE template<typename T>

const int MAX_ENTRY = 100;

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

	/*
	* BlackboardItem()
	* default constructor
	*/
	BlackboardItem() {};


	/*
	* ~BlackboardItem()
	* virtual function
	* default destructor
	*/
	virtual ~BlackboardItem() {};

};