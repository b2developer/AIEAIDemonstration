#include "BehaviourCondition.h"

//evaluates the condition of the behaviour
BehaviourResponse BehaviourCondition::update(float deltaTime)
{
	return evaluation(tree->agent);
}
