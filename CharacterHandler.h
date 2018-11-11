#ifndef CHARACTERHANDLER_H
#define CHARACTERHANDLER_H

#include "Includes.h"
#include "InputHandler.h"
#include "PhysicsHandler.h"

enum CharacterActions {CHARACTERACTION_RUNNING, CHARACTERACTION_WALKING, CHARACTERACTION_IDLE};

class CharacterHandler : public PhysicsObject{
public:
	InputHandler* inputHandler;

	float arm_back_tension;
	float arm_frnt_tension;
	float leg_back_tension;
	float leg_frnt_tension;

	float arm_back_rotation;
	float arm_frnt_rotation;
	float leg_back_rotation;
	float leg_frnt_rotation;

	bool direction; //true == right-facing, false == left-facing

	CharacterHandler(InputHandler* inputHandler, PhysicsHandler* physicsHandler, b2Vec2 position);
	int update(float deltaTime);
};

#endif