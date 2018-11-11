#ifndef PHYSHANDLER_H
#define PHYSHANDLER_H

#include "Includes.h"

class PhysicsHandler;

class PhysicsObject	{
	friend class PhysicsHandler;

public:
	b2Body* body;
	PhysicsHandler* physicsHandler;

	b2Vec2 dimensions; //Handy to have ready.

	PhysicsObject(PhysicsHandler* physicsHandler, b2Vec2 position, b2Vec2 dimensions);
	PhysicsObject(PhysicsHandler* physicsHandler, b2Vec2 position, b2Vec2 dimensions, float mass);
};

class PhysicsHandler {
	friend class PhysicsObject;
	b2Vec2 gravity;
	b2World world;

	float32 timeStep;
	int32 velocityIterations;
	int32 positionIterations;
public:
	std::vector<PhysicsObject*> physicsObjects;

	PhysicsHandler();
	int update(float deltaTime);

};

#endif