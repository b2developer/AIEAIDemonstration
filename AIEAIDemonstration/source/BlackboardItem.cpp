#include "BlackboardItem.h"
#include "BlackboardReference.h"

//destructor, tells all references that the item is gone
BlackboardItem::~BlackboardItem()
{
	//let all references know about this object no longer existing
	for (size_t i = 0; i < references.size(); i++)
	{
		references[i]->exists = false;
		references[i]->item = nullptr;
	}
}