#include "CharacterHandler.h"

CharacterHandler::CharacterHandler(InputHandler* inputHandler, PhysicsHandler* physicsHandler, Vec2 position) :
	inputHandler(inputHandler),
	currentAction(CharacterActions::IDLE),
	PhysicsObject(physicsHandler, position, Vec2(0.4f, 3.0f), 70.0f, false, false) {
	this->canJump = true;

	this->arm_back_tension = 0.0f;
	this->arm_frnt_tension = 0.0f;
	this->leg_back_tension = 0.0f;
	this->leg_frnt_tension = 0.0f;
	this->arm_back_rotation = 0.0f;
	this->arm_frnt_rotation = 0.0f;
	this->leg_back_rotation = 0.0f;
	this->leg_frnt_rotation = 0.0f;

	this->footstepAccumulator = 0.0f;
	this->footstepAccumulator_direction = false;

	this->playerGrip = 20.0f;
	this->playerSpeed = 5.0f;

	this->direction = true; //true == right-facing, false == left-facing

}

int CharacterHandler::update(float deltaTime) {
	this->controller(deltaTime);

	if (this->inputHandler->inputActivity.forward && !this->inputHandler->inputActivity.back) {
		this->direction = true;
	}
	if (!this->inputHandler->inputActivity.forward && this->inputHandler->inputActivity.back) {
		this->direction = false;
	}

	if (this->inputHandler->inputActivity.reset) {
		this->Teleport(Vec2(1.0f, 10.0f));
	}


	/*Do some incredibly basic character animations*/

	if (this->currentAction == WALKING) {
		this->footstepAccumulator += deltaTime * (this->playerSpeed / 2.0f) * (this->footstepAccumulator_direction ? -1.0f : 1.0f);
		if (this->footstepAccumulator >= 1.0f || this->footstepAccumulator <= 0.0f) {
			this->footstepAccumulator_direction = !this->footstepAccumulator_direction;
			
			if (this->footstepAccumulator < 0.0f) {
				this->footstepAccumulator = 0.0f;
			}
			else if (this->footstepAccumulator > 1.0f) {
				this->footstepAccumulator = 1.0f;
			}
		}
		
	}
	else {
		this->footstepAccumulator = 0.5f;
		this->footstepAccumulator_direction = false;
	}

	switch (this->currentAction) {
	case IDLE:
		ReduceToZero(this->arm_frnt_tension,  deltaTime);
		ReduceToZero(this->leg_back_tension,  deltaTime);
		ReduceToZero(this->leg_frnt_tension,  deltaTime);
		ReduceToZero(this->arm_back_tension,  deltaTime);
		ReduceToZero(this->arm_back_rotation, deltaTime * 60.0f);
		ReduceToZero(this->arm_frnt_rotation, deltaTime * 60.0f);
		ReduceToZero(this->leg_back_rotation, deltaTime * 60.0f);
		ReduceToZero(this->leg_frnt_rotation, deltaTime * 60.0f);
		break;
	case WALKING:
		this->arm_back_tension = this->footstepAccumulator * 0.5f + 0.5f;
		this->leg_back_tension = this->footstepAccumulator;
		this->leg_frnt_tension = 1.0f - this->footstepAccumulator;
		this->arm_frnt_tension = 1.0f - (this->footstepAccumulator * 0.5f);
		this->arm_back_rotation = 30.0f * ((this->footstepAccumulator * 2.0f) - 1.0f);
		this->arm_frnt_rotation = -30.0f * ((this->footstepAccumulator * 2.0f) - 1.0f);
		this->leg_back_rotation = -30.0f * ((this->footstepAccumulator * 2.0f) - 1.0f);
		this->leg_frnt_rotation = 30.0f * ((this->footstepAccumulator * 2.0f) - 1.0f);
	}
	
	return 0;
}

void CharacterHandler::controller(float deltaTime) {
	/*HORIZONTAL MOVEMENT*/
	this->currentAction = WALKING;
	if (this->inputHandler->inputActivity.forward) {
		if (!this->inputHandler->inputActivity.back) {
			if (this->movement.x < 0.0f) {
				this->movement.x = 0.0f;
			}
			this->movement.x = Min(this->movement.x + deltaTime * this->playerGrip, this->playerSpeed);
		}
	}
	else if (this->inputHandler->inputActivity.back) {
		if (!this->inputHandler->inputActivity.forward) {
			if (this->movement.x > 0.0f) {
				this->movement.x = 0.0f;
			}
			this->movement.x = Max(this->movement.x - deltaTime * this->playerGrip, -this->playerSpeed);
		}
	}
	else {
		this->currentAction = IDLE;
		ReduceToZero(this->movement.x, deltaTime * this->playerGrip);
	}
	if (this->touchingWall) {
		this->currentAction = IDLE;
	}

	/*JUMPING*/
	if (this->touchingGround) {

		if (this->canJump && this->inputHandler->inputActivity.jump) {
			this->ApplyForce(Vec2(0.0f, this->mass * 300.0f));
			this->canJump = false;
		}

		if (!this->canJump && !this->inputHandler->inputActivity.jump) {
			this->canJump = true;
		}
	}
}