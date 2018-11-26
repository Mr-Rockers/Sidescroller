#ifndef CHARACTERHANDLER_H
#define CHARACTERHANDLER_H

#include "Includes.h"
#include "InputHandler.h"
#include "PhysicsHandler.h"

enum CharacterActions {RUNNING, WALKING, IDLE};

class CharacterHandler : public PhysicsObject{
	bool canJump;
public:
	InputHandler* inputHandler;
	CharacterActions currentAction;

	float arm_back_tension;
	float arm_frnt_tension;
	float leg_back_tension;
	float leg_frnt_tension;
	float arm_back_rotation;
	float arm_frnt_rotation;
	float leg_back_rotation;
	float leg_frnt_rotation;

	float footstepAccumulator;
	bool footstepAccumulator_direction;

	//Essentially the player acceleration, but I have refrained from calling it that
	//in case it gets confused with parts of the physics engine.
	float playerGrip;
	//The player's speed.
	float playerSpeed;

	bool direction; //true == right-facing, false == left-facing

	CharacterHandler(InputHandler* inputHandler, PhysicsHandler* physicsHandler, Vec2 position);
	int update(float deltaTime);

	void controller(float deltaTime);
};

#endif