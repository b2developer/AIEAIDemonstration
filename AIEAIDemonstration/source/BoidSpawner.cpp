#include "BoidSpawner.h"
#include "Application2D.h"

#include "Transform.h"
#include "Box.h"
#include "Renderer.h"
#include "Entity.h"
#include "SteeringBehaviourManager.h"
#include "SteeringBehaviour.h"

#include "SeekBehaviour.h"
#include "FleeBehaviour.h"
#include "WanderBehaviour.h"
#include "PursueBehaviour.h"
#include "EvadeBehaviour.h"
#include "ArrivalBehaviour.h"
#include "ObstacleAvoidanceBehaviour.h"
#include "SeparationBehaviour.h"
#include "CohesionBehaviour.h"
#include "AlignmentBehaviour.h"

//adds the necessary components for a trading bot object
void BoidSpawner::addComponents(GameObject * creation)
{
	//add all components
	Transform* transform = new Transform();
	Box* box = new Box();
	Renderer* renderer = new Renderer();
	Entity* entity = new Entity();
	SteeringBehaviourManager* sbm = new SteeringBehaviourManager();

	transform->translation = Vector2((float)(rand() % 500), (float)(rand() % 500));
	transform->scale = Vector2(1.0f, 1.0f);
	transform->rotation = 0.0f;

	char texturePath[FILENAME_MAX];

	//load the boid texture for the renderer
	strcpy_s(texturePath, appPtr->resourceFolder);
	strcat_s(texturePath, "textures\\boid.png");
	renderer->load(texturePath);

	//set the box coordinates to match the texture
	box->min = Vector2((float)renderer->texture->getWidth(), (float)renderer->texture->getHeight()) * -0.5f;
	box->max = Vector2((float)renderer->texture->getWidth(), (float)renderer->texture->getHeight()) * 0.5f;

	//scale down the texture to 10% of the original size
	box->min = box->min * 0.1f;
	box->max = box->max * 0.1f;

	if (mode == BoidMode::MOUSE_FOLLOWER)
	{
		//give the steering behaviour manager all of it's behaviours
		SteeringBehaviour* seek = new SeekBehaviour();
		((SeekBehaviour*)seek)->target = &appPtr->mousePos;
		seek->sbm = sbm;

		WanderBehaviour* wander = new WanderBehaviour();
		((WanderBehaviour*)wander)->wanderRadius = 15.0f;
		((WanderBehaviour*)wander)->wanderDistance = 5.0f;
		((WanderBehaviour*)wander)->jitterAmount = 50.0f;
		wander->sbm = sbm;

		ArrivalBehaviour* arrival = new ArrivalBehaviour();
		((ArrivalBehaviour*)arrival)->arrivalRadius = 1.0f;
		((ArrivalBehaviour*)arrival)->target = &appPtr->mousePos;
		arrival->sbm = sbm;

		ObstacleAvoidanceBehaviour* avoidance = new ObstacleAvoidanceBehaviour();
		((ObstacleAvoidanceBehaviour*)avoidance)->aheadDistance = 200.0f;
		avoidance->sbm = sbm;

		SeparationBehaviour* separation = new SeparationBehaviour();
		((SeparationBehaviour*)separation)->neighbourhoodRadius = 50.0f;
		separation->sbm = sbm;

		CohesionBehaviour* cohesion = new CohesionBehaviour();
		((CohesionBehaviour*)cohesion)->neighbourhoodRadius = 200.0f;
		cohesion->sbm = sbm;

		AlignmentBehaviour* align = new AlignmentBehaviour();
		((AlignmentBehaviour*)align)->neighbourhoodRadius = 100.0f;
		((AlignmentBehaviour*)align)->neighbourhoodForwardDistance = 50.0f;
		align->sbm = sbm;

		seek->weight = 5.0f;
		wander->weight = 0.2f;
		arrival->weight = 0.0f;
		separation->weight = 40.0f;
		cohesion->weight = 0.6f;
		align->weight = 15.0f;
		avoidance->weight = 2.0f;

		sbm->behaviours.push_back(seek);
		sbm->behaviours.push_back(wander);
		sbm->behaviours.push_back(arrival);
		sbm->behaviours.push_back(separation);
		sbm->behaviours.push_back(cohesion);
		sbm->behaviours.push_back(align);
		sbm->behaviours.push_back(avoidance);
	}

	if (mode == BoidMode::PURSUER)
	{
		//give the steering behaviour manager all of it's behaviours
		PursueBehaviour* pursue = new PursueBehaviour();
		((PursueBehaviour*)pursue)->targetBoid = previousBoid;
		pursue->sbm = sbm;

		pursue->weight = 1.0f;

		sbm->behaviours.push_back(pursue);
	}

	if (mode == BoidMode::AVOID_BUT_FOLLOW)
	{
		//give the steering behaviour manager all of it's behaviours
		SteeringBehaviour* seek = new SeekBehaviour();
		((SeekBehaviour*)seek)->target = &appPtr->mousePos;
		seek->sbm = sbm;

		EvadeBehaviour* evade = new EvadeBehaviour();
		((EvadeBehaviour*)evade)->targetBoid = previousBoid;
		evade->sbm = sbm;

		seek->weight = 1.0f;
		evade->weight = 0.2f;

		sbm->behaviours.push_back(seek);
		sbm->behaviours.push_back(evade);
	}

	sbm->maxVelocity = 200.0f;
	sbm->maxAcceleration = 200.0f;

	//give the steering behaviour manager a blackboard to communicate with others
	sbm->enviroment = boidBlackboard;

	//linking the components together
	renderer->box = box;
	renderer->appPtr = appPtr;
	box->transform = transform;
	entity->transform = transform;
	entity->appPtr = appPtr;
	sbm->transform = transform;
	sbm->appPtr = appPtr;
	sbm->entity = entity;

	//link the components to the game object
	transform->container = creation;
	box->container = creation;
	renderer->container = creation;
	entity->container = creation;
	sbm->container = creation;

	//add the components to the list
	creation->components.push_back(transform);
	creation->components.push_back(box);
	creation->components.push_back(renderer);
	creation->components.push_back(entity);
	creation->components.push_back(sbm);

	//register the obstacle on the blackboard
	BlackboardReference* bref = boidBlackboard->registerItemWithReference<Boid, Boid>(0.0f);
	((BlackboardData<Boid, Boid>*)bref->item)->timeLocked = true;
	((BlackboardData<Boid, Boid>*)bref->item)->data.sbm = sbm;

	previousBoid = sbm;

}
