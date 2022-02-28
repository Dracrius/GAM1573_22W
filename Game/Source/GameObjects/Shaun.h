#pragma once

#include "Framework.h"
#include "DataTypes.h"
#include "Objects/GameObject.h"

enum PlayerActions
{
	IdleLeft,
	IdleRight,
	WalkLeft,
	WalkRight,
	JumpLeft,
	JumpRight,
	CrouchLeft,
	CrouchRight,
};

struct spriteFrames
{
    std::vector<fw::SpriteSheet::SpriteInfo*> crouch;
    std::vector<fw::SpriteSheet::SpriteInfo*> idleLeft;
    std::vector<fw::SpriteSheet::SpriteInfo*> idleRight;
	std::vector<fw::SpriteSheet::SpriteInfo*> walkLeft;
	std::vector<fw::SpriteSheet::SpriteInfo*> walkRight;

	spriteFrames(std::vector<fw::SpriteSheet::SpriteInfo*> crouchFrames, std::vector<fw::SpriteSheet::SpriteInfo*> iLeftFrames, std::vector<fw::SpriteSheet::SpriteInfo*> iRightFrames, std::vector<fw::SpriteSheet::SpriteInfo*> wLeftFrames, std::vector<fw::SpriteSheet::SpriteInfo*> wRightFrames) : crouch(crouchFrames), idleLeft(iLeftFrames), idleRight(iRightFrames), walkLeft(wLeftFrames), walkRight(wRightFrames) {}
	spriteFrames() {}
};

class PlayerController;
class Tilemap;

class Shaun : public fw::GameObject
{
public:
	Shaun(fw::Scene* pScene, fw::Mesh* pMesh, fw::Material* pMaterial, vec2 pos, PlayerController* pController);
    virtual ~Shaun();

    virtual void Update(float deltaTime) override;
    //virtual void Draw() override;

    // Setters.
    void SetSpriteSheet(fw::SpriteSheet* pSpriteSheet) { m_pSpriteSheet = pSpriteSheet; SetAnimations(); }
    void SetTilemap(Tilemap* pTilemap) { m_pTilemap = pTilemap; }
    void SetAnimations();

    void CycleAnimFrames();

protected:
    PlayerController* m_pPlayerController = nullptr;

	spriteFrames m_shaunFrames;

	PlayerActions m_playersLastAction = IdleRight;
	PlayerActions m_playersCurrentAction = IdleRight;

    int m_animFrame = 0;
    float m_animationFrameTimer = 0.f;
	float m_animationTime = 0.f;
    float m_jumpTimer = 0.f;
    vec3 m_velocity;

	bool m_onGround = false;

    fw::SpriteSheet* m_pSpriteSheet = nullptr;
    Tilemap* m_pTilemap = nullptr;
};
