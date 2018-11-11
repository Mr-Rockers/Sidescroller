#include "PhysicsHandler.h"

PhysicsHandler::PhysicsHandler() :
gravity(0.0f, -9.81f),
world(gravity),
timeStep(1.0f / 60.0f),
velocityIterations(8),
positionIterations(3){

}

int PhysicsHandler::update(float deltaTime) {
	world.Step(this->timeStep, this->velocityIterations, this->positionIterations);

	return 0;
}

PhysicsObject::PhysicsObject(PhysicsHandler* physicsHandler, b2Vec2 position, b2Vec2 dimensions) :
physicsHandler(physicsHandler),
dimensions(dimensions){
	b2BodyDef bodyDef;
	bodyDef.position = position;
	this->body = this->physicsHandler->world.CreateBody(&bodyDef);

	b2PolygonShape box;
	box.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f);


	body->CreateFixture(&box, 0.0f);
	/*std::cout << "Static Position: [" << bodyDef.position.x << "," << bodyDef.position.y << "]\n";
	for (int i = 0; i < 4; i++) {
		std::cout << " - Vertex Position: [" << box.m_vertices[i].x << "," << box.m_vertices[i].y << "]\n";
	}*/
}

PhysicsObject::PhysicsObject(PhysicsHandler* physicsHandler, b2Vec2 position, b2Vec2 dimensions, float mass) :
physicsHandler(physicsHandler),
dimensions(dimensions){
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = position;
	bodyDef.fixedRotation = true;
	this->body = this->physicsHandler->world.CreateBody(&bodyDef);

	b2PolygonShape box;
	box.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f);


	b2FixtureDef fixtureDef;
	fixtureDef.density = mass;
	fixtureDef.friction = 0.3;
	fixtureDef.shape = &box;

	body->CreateFixture(&fixtureDef);
	/*std::cout << "Dynamic Position: [" << bodyDef.position.x << "," << bodyDef.position.y << "]\n";
	for (int i = 0; i < 4; i++) {
		std::cout << " - Vertex Position: [" << box.m_vertices[i].x << "," << box.m_vertices[i].y << "]\n";
	}*/
}
