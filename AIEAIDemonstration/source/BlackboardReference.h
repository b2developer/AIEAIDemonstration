#pragma once
#include "BlackboardItem.h"

#define TEMPLATE template<typename T>
#define TEMPLATE2 template<typename T, typename U>

/*
* class BlackboardReference
*
* a class that allows other systems
* to safely access blackboard data, a boolean flag
* gets set if the item gets deleted
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class BlackboardReference
{
public:

	//the item being referenced
	BlackboardItem* item;

	//flag indicating if the item exists
	bool exists = true;

	/*
	* BlackboardReference()
	* default constructor
	*/
	BlackboardReference() {};


	/*
	* ~BlackboardReference()
	* default destructor
	*/
	~BlackboardReference() {};
};