#include "BlackboardItem.h"

#define TEMPLATE template<typename T>
#define TEMPLATE2 template<typename T, typename U>

/*
* class BlackboardData
* template with two types
* child class of BlackboardItem
*
* a type of information on the blackboard
* that contains information posted by one agent
*
* other agents can then respond by depositing answers
*
* finally an arbiter can then observe
* the answers and deduct the best solution
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
TEMPLATE2
class BlackboardData : public BlackboardItem
{
public:

	//information held
	T data;

	//list of responses from other agents
	std::vector<U> answers;

	/*
	* BlackboardData()
	* default constructor
	*/
	BlackboardData() {}


	/*
	* ~BlackboardData()
	* default destructor
	*/
	~BlackboardData() {}
};