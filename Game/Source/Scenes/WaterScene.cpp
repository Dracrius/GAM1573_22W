#include "Framework.h"

#include "WaterScene.h"
#include "DataTypes.h"
#include "GameObjects/Player.h"
#include "GameObjects/Cube.h"
#include "Game.h"

WaterScene::WaterScene(Game* pGame) : fw::Scene(pGame)
{
    m_pPhysicsWorld = new fw::PhysicsWorldBox2D();
    m_pPhysicsWorld->SetGravity(vec2(0.f, -9.8f));

    m_pCamera = new fw::Camera(this, vec3(vec2(1.5f * 10, 1.5f * 10) / 2) + vec3(0.f, 0.f, -20.f));

    vec3 rot = vec3(0.f, 0.f, 0.f); //vec3(-80.f, 0.f, 0.f);
    vec3 pos = vec3((1.5f * 10) / 2, (1.5f * 10) / 2, -5.f) + vec3(0.f,-2.f,2.f);

    Cube* pPlane = new Cube(this, pGame->GetMesh("Plane"), pGame->GetMaterial("Water"), pos, rot);
    m_Objects.push_back(pPlane);
}

WaterScene::~WaterScene()
{
}

void WaterScene::StartFrame(float deltaTime)
{
}

void WaterScene::OnEvent(fw::Event* pEvent)
{
}

void WaterScene::Update(float deltaTime)
{
    Scene::Update(deltaTime);

    //float time = (float)fw::GetSystemTimeSinceGameStart() * 10;
}
