#ifndef PHYSHANDLER_H
#define PHYSHANDLER_H

#include "Includes.h"

struct PhysicsHandler;
enum CollisionSide { TOP = 0, BOTTOM = 1, LEFT = 2, RIGHT = 3, NONE = 4 };

struct PhysicsObject {
	PhysicsHandler* physicsHandler;

	Vec2 acceleration;
	Vec2 movement; //Hooks into velocity as an acceleration. 
	Vec2 velocity;
	Vec2 position_previous;
	Vec2 position;
	Vec2 dimensions;

	Vec2 velocity_smooth; //Used for rendering only.
	Vec2 position_smooth; //Used for rendering only.

	float mass;

	bool ignoreCollisions; //Ignore all collisions. Basically reduces the PhysicsHandler to a position container only.
	bool isFixed;
	bool touchingGround;
	bool touchingWall;

	void updateTransform();

	PhysicsObject(PhysicsHandler* physicsHandler, Vec2 position, Vec2 dimensions, bool ignoreCollisions);
	PhysicsObject(PhysicsHandler* physicsHandler, Vec2 position, Vec2 dimensions, float mass, bool isFixed, bool ignoreCollisions);

	virtual void onCollision(CollisionSide side); //Overloadable

	void Teleport(Vec2 position);
	void ApplyForce(Vec2 newtons);
};

struct PhysicsHandler {
	Vec2 gravity;
	float timestep;
	float timestepAccumulator;
	std::vector<PhysicsObject*> world;

	PhysicsHandler();
	int update(float deltaTime);

	void fixedUpdate();

};

bool SORT_FUNCTION_PHYSICSOBJECT_AREA(std::pair<PhysicsObject*, float> a, std::pair<PhysicsObject*, float> b);

#endif