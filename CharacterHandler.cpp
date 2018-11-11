#include "CharacterHandler.h"

CharacterHandler::CharacterHandler(InputHandler* inputHandler, PhysicsHandler* physicsHandler, b2Vec2 position) :
	inputHandler(inputHandler),
	PhysicsObject(physicsHandler, position, b2Vec2(1.0f, 3.0f), 70.0f) {
	this->arm_back_tension = 0.0f;
	this->arm_frnt_tension = 0.0f;
	this->leg_back_tension = 0.0f;
	this->leg_frnt_tension = 0.0f;

	this->arm_back_rotation = 0.0f;
	this->arm_frnt_rotation = 0.0f;
	this->leg_back_rotation = 0.0f;
	this->leg_frnt_rotation = 0.0f;

	this->direction = true; //true == right-facing, false == left-facing
	this->body->GetFixtureList()->SetFriction(0.3f);
}

int CharacterHandler::update(float deltaTime) {

	if (!(this->inputHandler->inputActivity.forward && this->inputHandler->inputActivity.back)) {
		if (this->inputHandler->inputActivity.forward) {
			this->direction = false;
			this->body->ApplyLinearImpulseToCenter(b2Vec2(10.0f, 0.0f), true);
		}
		if (this->inputHandler->inputActivity.back) {
			this->direction = true;
			this->body->ApplyLinearImpulseToCenter(b2Vec2(-10.0f, 0.0f), true);
		}
	}
	return 0;
}
