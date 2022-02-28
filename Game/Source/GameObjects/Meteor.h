#pragma once

#include "Framework.h"
#include "DataTypes.h"
#include "Objects/GameObject.h"

class Meteor : public fw::GameObject
{
public:
	Meteor(fw::Scene* pScene, fw::Mesh* pMesh, fw::Material* pMaterial, vec2 pos, vec3 rot);
    virtual ~Meteor();

    virtual void Update(float deltaTime) override;
    //virtual void Draw() override;

protected:
};