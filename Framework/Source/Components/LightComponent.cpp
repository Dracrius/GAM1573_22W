#include "CoreHeaders.h"

#include "LightComponent.h"

namespace fw {

LightComponent::LightComponent(LightType type, Color4f color, float radius, float powerFactor)
{
	m_light = { type, color * 0.1f, color, color * 0.5f, radius, powerFactor };
}

LightComponent::LightComponent(LightType type, Color4f color, float radius, float powerFactor, float spotCutoff) : m_spotCutoff(spotCutoff)
{
    m_light = { type, color * 0.1f, color, color * 0.5f, radius, powerFactor };
}

LightComponent::~LightComponent()
{

}

void LightComponent::SetDiffuse(Color4f diffuse)
{
	m_light.diffuse = diffuse;
	m_light.ambient = diffuse * 0.1f;
	m_light.specular = diffuse * 0.5f;
}

} // namespace fw
