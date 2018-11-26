#include "PhysicsHandler.h"

PhysicsHandler::PhysicsHandler() :
gravity(Vec2(0.0f, -9.81f)),
timestep(1.0f / 60.0f),
timestepAccumulator(0.0f),
world(std::vector<PhysicsObject*>()){

}

int PhysicsHandler::update(float deltaTime) {
	timestepAccumulator += deltaTime;
	while (timestepAccumulator >= timestep) {
		timestepAccumulator -= timestep;
		this->fixedUpdate();
	}

	float interpAlpha = timestepAccumulator / deltaTime;
	for (PhysicsObject* entity : this->world) {
		if (entity->isFixed || entity->ignoreCollisions) continue;

		if (entity->velocity_smooth.x != entity->velocity.x) {
			entity->velocity_smooth.x =
				entity->velocity.x * interpAlpha +
				(entity->velocity_smooth.x * (1.0f - interpAlpha));
		}
		if (entity->velocity_smooth.y != entity->velocity.y) {
			entity->velocity_smooth.y =
				entity->velocity.y * interpAlpha +
				(entity->velocity_smooth.y * (1.0f - interpAlpha));
		}

		if (entity->position_smooth.x != entity->position.y) {
			entity->position_smooth.x =
				entity->position.x * interpAlpha +
				(entity->position_smooth.x * (1.0f - interpAlpha));
		}
		if (entity->position_smooth.y != entity->position.y) {
			entity->position_smooth.y =
				entity->position.y * interpAlpha +
				(entity->position_smooth.y * (1.0f - interpAlpha));
		}
	}

	return 0;
}

void PhysicsHandler::fixedUpdate() {
	for (PhysicsObject* entity : this->world) {
		if (entity->isFixed) continue; ///Fixed entities shouldn't move.
		if (entity->ignoreCollisions) continue; ///Entities that ignore collisions shouldn't check for collisions!

		entity->touchingGround = false; ///If we are touching the ground, then this will be set to true.
		entity->touchingWall = false;
		
		//Update Positions
		entity->updateTransform();

		//Find Collisions.
		std::vector<std::pair<PhysicsObject*, float>> currentlyColliding = std::vector<std::pair<PhysicsObject*, float>>();
		//	-Look for collisions.
		for (PhysicsObject* collision : this->world) {
			if (entity == collision) continue; ///Entity and Collision are the same.
			if (collision->ignoreCollisions) continue; ///Entities avoiding collisions shouldn't be checked.

			//	-Retrieve entity's past and present bounding boxes.
			AABB entityAABB = AABB(entity->position, entity->dimensions / 2.0f);
			AABB previousAABB = AABB(entity->position_previous, entity->dimensions / 2.0f);
			AABB collisionAABB = AABB(collision->position, collision->dimensions / 2.0f);
			if (AABB::Intersect(entityAABB, collisionAABB)) {

				//Find area of intersection.
				float left = Max(entityAABB.getLeft(), collisionAABB.getLeft());
				float right = Min(entityAABB.getRight(), collisionAABB.getRight());
				float top = Max(entityAABB.getTop(), collisionAABB.getTop());
				float bottom = Min(entityAABB.getBottom(), collisionAABB.getBottom());

				float unionArea = 0.0f;
				if (left < right && bottom < top) {
					float intersection = (right - left) * (top - bottom);
					unionArea =
						(entityAABB.getWidth() * entityAABB.getHeight()) +
						(collisionAABB.getWidth() * collisionAABB.getHeight()) -
						intersection;
				}
				if (unionArea != 0.0f) {
					currentlyColliding.push_back(std::pair<PhysicsObject*, float>(collision, unionArea));
				}
			}
		}

		//Sort Collisions
		if (!currentlyColliding.empty()) {
			std::sort(currentlyColliding.begin(), currentlyColliding.end(), SORT_FUNCTION_PHYSICSOBJECT_AREA);


			//Resolve Collisions
			for (std::pair<PhysicsObject*, float> collisionPair : currentlyColliding) {
				PhysicsObject* collision = collisionPair.first;
				//	-Retrieve entity's past and present bounding boxes.
				AABB entityAABB = AABB(entity->position, entity->dimensions / 2.0f);
				AABB previousAABB = AABB(entity->position_previous, entity->dimensions / 2.0f);
				AABB collisionAABB = AABB(collision->position, collision->dimensions / 2.0f);
				if (AABB::Intersect(entityAABB, collisionAABB)) {
					entityAABB = AABB(entity->position, entity->dimensions / 2.0f);

					//Find differences between all the sides of the previous transform and the collision transform.
					float n0_t_difference = abs(previousAABB.getBottom() - collisionAABB.getTop());
					float n0_b_difference = abs(previousAABB.getTop() - collisionAABB.getBottom());
					float n0_l_difference = abs(previousAABB.getRight() - collisionAABB.getLeft());
					float n0_r_difference = abs(previousAABB.getLeft() - collisionAABB.getRight());

					float n1_t_difference = abs(entityAABB.getBottom() - collisionAABB.getTop());
					float n1_b_difference = abs(entityAABB.getTop() - collisionAABB.getBottom());
					float n1_l_difference = abs(entityAABB.getRight() - collisionAABB.getLeft());
					float n1_r_difference = abs(entityAABB.getLeft() - collisionAABB.getRight());

					//Find shortest collision side.
					CollisionSide colSide = (CollisionSide)FindInArray(new float[4]{ n0_t_difference, n0_b_difference, n0_l_difference, n0_r_difference }, 4, SMALLEST);

					entity->position_previous = entity->position; ///Update previous position.
					switch (colSide) {
					case CollisionSide::TOP:
						///Top side of collisionAABB.
						entity->position.y += n1_t_difference;
						entity->velocity.y = Max(entity->velocity.y, 0.0f);
						entity->movement.y = Max(entity->movement.y, 0.0f);
						entity->touchingGround = true;
						break;
					case CollisionSide::BOTTOM:
						///Bottom side of collisionAABB.
						entity->position.y -= n1_b_difference;
						entity->velocity.y = Min(entity->velocity.y, 0.0f);
						entity->movement.y = Min(entity->movement.y, 0.0f);
						break;
					case CollisionSide::LEFT:
						///Left side of collisionAABB.
						entity->position.x -= n1_l_difference;
						entity->velocity.x = Min(entity->velocity.x, 0.0f);
						entity->movement.x = Min(entity->movement.x, 0.0f);
						entity->touchingWall = true;
						break;
					case CollisionSide::RIGHT:
						///Right side of collisionAABB.
						entity->position.x += n1_r_difference;
						entity->velocity.x = Max(entity->velocity.x, 0.0f);
						entity->movement.x = Max(entity->movement.x, 0.0f);
						entity->touchingWall = true;
						break;
					default:
						break;
					}
					entity->onCollision(colSide);
				}
				else {
					break;
				}
			}
		}
	}
}


PhysicsObject::PhysicsObject(PhysicsHandler* physicsHandler, Vec2 position, Vec2 dimensions, float mass, bool isFixed, bool ignoreCollisions) :
physicsHandler(physicsHandler),
acceleration(),
movement(),
velocity(),
velocity_smooth(),
position(position),
position_previous(position),
position_smooth(position),
dimensions(dimensions),
mass(mass),
isFixed(isFixed),
ignoreCollisions(ignoreCollisions),
touchingGround(false),
touchingWall(false){
	this->physicsHandler->world.push_back(this);
}
PhysicsObject::PhysicsObject(PhysicsHandler* physicsHandler, Vec2 position, Vec2 dimensions, bool ignoreCollisions) :
	PhysicsObject(physicsHandler, position, dimensions, 0.0f, true, ignoreCollisions) {}


void PhysicsObject::onCollision(CollisionSide side) {
}

void PhysicsObject::Teleport(Vec2 position){
	this->movement = Vec2();
	this->velocity = Vec2();
	this->acceleration = Vec2();
	this->position = position;
	this->position_previous = position;
}

void PhysicsObject::ApplyForce(Vec2 newtons) {
	//a = f / m
	if (this->ignoreCollisions)
		return;

	this->acceleration += newtons / this->mass;
}

void PhysicsObject::updateTransform(){

	if (this->isFixed || this->ignoreCollisions) {
		return;
	}

	//a = f / m (Keep this in mind whenever applying Force calculations.)
	Vec2 localAcceleration = this->acceleration;
	this->acceleration = Vec2(); ///Reset acceleration.

	//Apply gravity.
	if (this->mass != 0) { ///0 Mass is also treated as Infinite Mass
		localAcceleration += this->physicsHandler->gravity;
	}
	//v = u + at
	this->velocity_smooth = this->velocity;
	this->velocity += (localAcceleration) * this->physicsHandler->timestep;
	//this->velocity.y = -5.0f;

	//p(n) = p(n-1)+(v * t)
	this->position_smooth = this->position;
	this->position_previous = this->position;
	this->position += (this->velocity + this->movement) * this->physicsHandler->timestep;
}
bool SORT_FUNCTION_PHYSICSOBJECT_AREA(std::pair<PhysicsObject*, float> a, std::pair<PhysicsObject*, float> b){
	return a.second > b.second;
}
