#include "CoreHeaders.h"
#include "PhysicsWorldBox2D.h"
#include "PhysicsBodyBox2D.h"
#include "Objects/GameObject.h"
#include "Events/EventManager.h"
#include "Events/Event.h"

namespace fw {

void ContactListenerBox2D::BeginContact(b2Contact* contact)
{
	b2Fixture* pFixtureA = contact->GetFixtureA();
	b2Fixture* pFixtureB = contact->GetFixtureB();

	b2Body* pBodyA = pFixtureA->GetBody();
	b2Body* pBodyB = pFixtureB->GetBody();

	GameObject* pObjectA = reinterpret_cast<GameObject*>(pBodyA->GetUserData().pointer);
	GameObject* pObjectB = reinterpret_cast<GameObject*>(pBodyB->GetUserData().pointer);

	CollisionEvent* pCollisionEvent = new CollisionEvent(pObjectA, pObjectB, ContactState::Begin);
	m_pEventManager->AddEvent(pCollisionEvent);
}

void ContactListenerBox2D::EndContact(b2Contact* contact)
{
}

void ContactListenerBox2D::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
}

void ContactListenerBox2D::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
}

PhysicsWorldBox2D::PhysicsWorldBox2D(EventManager* pEventManager) : PhysicsWorld(pEventManager)
{
	m_pWorld = new b2World(c_defaultGravity);

	m_pContactListener = new ContactListenerBox2D(pEventManager);

	m_pWorld->SetContactListener(m_pContactListener);
}

PhysicsWorldBox2D::~PhysicsWorldBox2D()
{
	delete m_pContactListener;
	delete m_pWorld;
}

void PhysicsWorldBox2D::Update(float deltaTime)
{
	m_pWorld->Step(deltaTime, 8, 3);
}

void PhysicsWorldBox2D::SetGravity(vec3 gravity)
{
	m_pWorld->SetGravity(b2Vec2(gravity.x, gravity.y));
}

PhysicsBody* PhysicsWorldBox2D::CreateBody(GameObject* owner, bool isDynamic, vec3 size, float density, vec3 pos, vec3 rot)
{
    b2BodyDef bodydef;
    bodydef.position = b2Vec2(pos.x, pos.y);
    bodydef.angle = rot.z;

    if (isDynamic)
    {
        bodydef.type = b2_dynamicBody;
    }
    else
    {
        bodydef.type = b2_staticBody;
    }

    bodydef.userData.pointer = reinterpret_cast<uintptr_t>(owner);

    b2PolygonShape shape;
    shape.SetAsBox(size.x / 2, size.y / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = density;

    b2Body* body = m_pWorld->CreateBody(&bodydef);
    body->CreateFixture(&fixtureDef);

    PhysicsBody* physicsBody = new PhysicsBodyBox2D(body);

    return physicsBody;
}

} // namespace fw
