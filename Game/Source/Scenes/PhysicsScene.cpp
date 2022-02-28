#include "Framework.h"

#include "PhysicsScene.h"
#include "DataTypes.h"
#include "GameObjects/PlayerController.h"
#include "GameObjects/Player.h"
#include "Game.h"
#include "DefaultSettings.h"

PhysicsScene::PhysicsScene(Game* pGame) : fw::Scene(pGame)
{
	m_pPhysicsWorld = new fw::PhysicsWorldBox2D(pGame->GetFramework()->GetEventManager());
	m_pPhysicsWorld->SetGravity(c_gravity);

	m_pCamera = new fw::Camera(this, c_centerOfScreen + c_cameraOffset);

	m_pPlayerController = new PlayerController();

	vec3 pos = c_centerOfScreen + vec3(1.5f, 1.f, 2.f);
	vec3 rot = vec3(90.f, 0.f, 0.f);

	fw::GameObject* pBackground = new fw::GameObject(this, pos, rot);
	pBackground->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Background"), m_pResourceManager->GetMaterial("Background")));
	pBackground->SetScale(vec3(18.8f, 0.f, 10.f));
	pBackground->SetName("Background");
	m_Objects.push_back(pBackground);

	for (int i = 0; i < 6; i++)
	{
		vec3 pos = c_centerOfScreen;
		if (i < 3)
		{
			pos += vec3(1.f - (float)i, 0.f, 0.f);
		}
		else if (i >= 3 && i < 5)
		{
			pos += vec3(3.5f - (float)i, 1.f, 0.f);
		}
		else
		{
			pos += vec3(0.f, 2.f, 0.f);
		}

		std::string name = "Numbered Box " + i;
		fw::GameObject* pBox = new fw::GameObject(this, pos, vec3());
		pBox->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("Cube")));
		pBox->SetScale(vec3(0.5f, 0.5f, 0.5f));
		pBox->CreateBody(m_pPhysicsWorld, true, vec3(1.0f, 1.0f, 1.0f), 1.f);
		pBox->SetName(name);
		m_Objects.push_back(pBox);
	}


	//Platform
	{
		fw::GameObject* pPlatform = new fw::GameObject(this, c_centerOfScreen + vec3(0.f, -4.5f, 0.f), vec3(0.f, 0.f, 0.f));

		fw::MeshComponent* pPlatformMesh = new fw::MeshComponent(m_pResourceManager->GetMesh("Platform"), m_pResourceManager->GetMaterial("PlatformCenter"));

		//pPlatformMesh->SetUVScale(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Ground_02")->uvScale);
		//pPlatformMesh->SetUVOffset(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Ground_02")->uvOffset);

		pPlatform->AddComponent(pPlatformMesh);
		pPlatform->SetScale(vec3(20.f, 2.f, 0.f));
		pPlatform->CreateBody(m_pPhysicsWorld, false, vec3(20.0f, 2.0f, 2.0f), 1.f);
		pPlatform->SetName("Platform");
		m_Objects.push_back(pPlatform);

		fw::GameObject* pLeftEdge = new fw::GameObject(this, c_centerOfScreen + vec3(-10.f, -4.5f, 0.f), vec3());

		fw::MeshComponent* pLeftEdgeMesh = new fw::MeshComponent(m_pResourceManager->GetMesh("Sprite"), m_pResourceManager->GetMaterial("NiceDaysWalk"));

		pLeftEdgeMesh->SetUVScale(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Ground_01")->uvScale);
		pLeftEdgeMesh->SetUVOffset(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Ground_01")->uvOffset);
		pLeftEdge->AddComponent(pLeftEdgeMesh);
		pLeftEdge->SetScale(vec3(2.f, 2.f, 0.f));
		pLeftEdge->CreateBody(m_pPhysicsWorld, false, vec3(2.0f, 2.0f, 2.0f), 1.f);
		pLeftEdge->SetName("Platform Left Edge");
		m_Objects.push_back(pLeftEdge);

		fw::GameObject* pRightEdge = new fw::GameObject(this, c_centerOfScreen + vec3(10.f, -4.5f, 0.f), vec3());

		fw::MeshComponent* pRightEdgeMesh = new fw::MeshComponent(m_pResourceManager->GetMesh("Sprite"), m_pResourceManager->GetMaterial("NiceDaysWalk"));

		pRightEdgeMesh->SetUVScale(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Ground_03")->uvScale);
		pRightEdgeMesh->SetUVOffset(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Ground_03")->uvOffset);
		pRightEdge->AddComponent(pRightEdgeMesh);
		pRightEdge->SetScale(vec3(2.f, 2.f, 0.f));
		pRightEdge->CreateBody(m_pPhysicsWorld, false, vec3(2.0f, 2.0f, 2.0f), 1.f);
		pRightEdge->SetName("Platform Left Edge");
		m_Objects.push_back(pRightEdge);
	}

	Player* pPlayer = new Player(this, m_pResourceManager->GetMesh("Sprite"), m_pResourceManager->GetMaterial("Sokoban"), vec2(7.5f, 12.0f), m_pPlayerController);
	pPlayer->SetSpriteSheet(m_pResourceManager->GetSpriteSheet("Sprites"));
	pPlayer->CreateBody(m_pPhysicsWorld, true, vec3(c_playerCollider.x, c_playerCollider.y, c_playerCollider.y), 1.f);
	pPlayer->SetName("Player");
	m_Objects.push_back(pPlayer);

	m_pCamera->AttachTo(m_Objects.back());
	m_pCamera->SetThirdPerson(c_cameraOffset + vec3(0.f, 4.5f, 0.f));
	m_pCamera->SetAspectRatio(c_aspectRatio);
}

PhysicsScene::~PhysicsScene()
{
    delete m_pPlayerController;
}

void PhysicsScene::StartFrame(float deltaTime)
{
    m_pPlayerController->StartFrame();
}

void PhysicsScene::OnEvent(fw::Event* pEvent)
{
    m_pPlayerController->OnEvent(pEvent);

    fw::Scene::OnEvent(pEvent);
}

void PhysicsScene::Update(float deltaTime)
{
    Scene::Update(deltaTime);

	ControlsMenu();
}

void PhysicsScene::ControlsMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::BeginMenu("Options"))
			{
				if (ImGui::MenuItem("Reset Background Color", "Ctrl+R"))
				{
					Game* pGame = static_cast<Game*>(m_pGame);

					pGame->ResetBackgroundColor(false);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings"))
		{
			if (ImGui::MenuItem("Enable Debug Draw", "", &m_debugDraw)) {}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Controls"))
		{
			ImGui::Text("Left = <- or A");
			ImGui::Text("Right = -> or D");
			ImGui::Text("Jump = Up or Space");
			ImGui::Text("Teleport = Z");

			ImGui::EndMenu();
		}
		ImGui::MenuItem("Physics Scene", NULL, false, false);

		ImGui::EndMainMenuBar();
	}
}
