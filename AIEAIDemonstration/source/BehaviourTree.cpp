#include "BehaviourTree.h"

//destructor, deletes the root node
BehaviourTree::~BehaviourTree()
{
	delete root;
}


//called once per frame, updates the root node
void BehaviourTree::update(float deltaTime)
{
	//don't update the root if it hasn't been assigned
	if (root == nullptr)
	{
		return;
	}

	//update the root
	root->update(deltaTime);
}
