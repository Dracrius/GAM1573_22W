#include "CoreHeaders.h"

#include "ComponentManager.h"
#include "Component.h"
#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"
#include "Components/PhysicsBodyComponent.h"
#include "Objects/GameObject.h"

namespace fw {

ComponentManager::ComponentManager()
{
}

void ComponentManager::Update(float deltaTime)
{
    for (Component* pComponent : m_Components[PhysicsBodyComponent::GetStaticType()])
    {
        PhysicsBodyComponent* pPhysicsBody = static_cast<PhysicsBodyComponent*>(pComponent);
        pPhysicsBody->Update(deltaTime);
    }
}

void ComponentManager::Draw(Camera* pCamera)
{
	for (Component* pComponent : m_Components[TransformComponent::GetStaticType()])
	{
		TransformComponent* pTransform = static_cast<TransformComponent*>(pComponent);
		pTransform->UpdateWorldTransform();
	}

    for (Component* pComponent : m_Components[MeshComponent::GetStaticType()])
    {
        MeshComponent* pMeshComponent = static_cast<MeshComponent*>(pComponent);

        // Create rotation matrix to rotate normals.
        vec3 rot = pMeshComponent->GetGameObject()->GetTransform()->GetRotation();
        matrix normalMatrix;
        normalMatrix.CreateRotation(rot);

        // Get the world matrix.
        const matrix& worldTransform = pMeshComponent->GetGameObject()->GetTransform()->GetWorldTransform();

        // Draw our mesh.
        pMeshComponent->Draw(pCamera, worldTransform, normalMatrix);
    }
}

void ComponentManager::AddComponent(Component* pComponent)
{
    std::vector<Component*>& list = m_Components[pComponent->GetType()];

    // Assert that the component *was not* already in the list.
    assert(std::find(list.begin(), list.end(), pComponent) == list.end());

    list.push_back( pComponent );
}

void ComponentManager::RemoveComponent(Component* pComponent)
{
    std::vector<Component*>& list = m_Components[pComponent->GetType()];

    // Assert that the component *was* in the list.
    assert(std::find(list.begin(), list.end(), pComponent) != list.end());

    list.erase(std::remove(list.begin(), list.end(), pComponent), list.end());
}

} // namespace fw
