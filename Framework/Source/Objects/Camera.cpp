#include "CoreHeaders.h"

#include "Camera.h"
#include "Objects/Scene.h"
#include "FWCore.h"

namespace fw {

Camera::Camera(Scene* pScene, vec3 pos) : GameObject( pScene,  pos, vec3() )
{
}

Camera::Camera(Scene* pScene, vec3 pos, vec3 lookAtPos) : GameObject(pScene, pos, vec3()), m_lookAtPos(lookAtPos), m_lockView(true)
{
}

Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
	if (m_shaking)
	{
		m_shakeTimer -= deltaTime;

		if (m_shakeTimer <= 0)
		{
			m_shakeTimer = 0.5f;
			m_shakeOffset = 0.f;
			m_shaking = false;
		}
		else if (m_shakeTimer > 0.375f || m_shakeTimer < 0.125f)
		{
			m_shakeOffset -= deltaTime;
		}
		else if(m_shakeTimer < 0.375f && m_shakeTimer > 0.125f)
		{
			m_shakeOffset += deltaTime;
		}
	}
	if (m_perspectiveMode) //Check that Perspective Mode is enabled
	{
		m_ProjecMatrix.CreatePerspectiveVFoV(45.f, m_aspectRatio, 0.01f, 100.f);  //45.f = Vertical FOV
		//m_ProjecMatrix.CreatePerspectiveHFoV(90.f, m_aspectRatio, 0.01f, 100.f);  //90.f = Horizontal FOV
	}
	else //If not use Ortho Projection
	{
		m_ProjecMatrix.CreateOrtho(-8 * m_aspectRatio, 8 * m_aspectRatio, -8, 8, 0.01f, 100.f);
	}

    if (m_pCameraOperator) //Check if the Camera is Attached to an Object ie. has a Camera Operatior
    {
		m_pTransform->SetPosition(m_pCameraOperator->GetPosition() + vec3(m_offset.x, m_offset.y + m_shakeOffset, m_offset.z));
    }

    if (m_lockView) //Check if the view has been locked to a position
    {
        m_ViewMatrix.CreateLookAtView(m_pTransform->GetPosition(), vec3(0.f, 1.f, 0.f), m_lookAtPos + vec3(0.f, m_shakeOffset, 0.f));
    }
    else //Otherwise look at the Camera's Position
    {
        m_ViewMatrix.CreateLookAtView(m_pTransform->GetPosition(), vec3(0.f, 1.f, 0.f), vec3(m_pTransform->GetPosition().x, m_pTransform->GetPosition().y, 0.f) + vec3(0.f, m_shakeOffset, 0.f));
    }
}

matrix Camera::GetViewMatrix()
{
    return m_ViewMatrix;
}

matrix Camera::GetProjecMatrix()
{
    return m_ProjecMatrix;
}

void Camera::Hack_ThirdPersonCam(FWCore* pFramework, float deltaTime)
{
	float speed = 90.f;
	float distance = 10.f;

	vec3 rot = m_pTransform->GetRotation();

	if (pFramework->IsKeyDown('J') || pFramework->IsKeyDown(37))
	{
		rot.y += deltaTime * speed;
	}
	if (pFramework->IsKeyDown('L') || pFramework->IsKeyDown(39))
	{
		rot.y -= deltaTime * speed;
	}

	if (pFramework->IsKeyDown('I') || pFramework->IsKeyDown(38))
	{
		rot.x -= deltaTime * speed;
	}
	if (pFramework->IsKeyDown('K') || pFramework->IsKeyDown(40))
	{
		rot.x += deltaTime * speed;
	}

	m_ViewMatrix.SetIdentity();
	m_ViewMatrix.Translate(vec3(0.f,0.f, -distance));
	m_ViewMatrix.Rotate(rot.x, 1, 0, 0);
	m_ViewMatrix.Rotate(rot.y, 0, 1, 0);
	m_ViewMatrix.Translate(m_pTransform->GetPosition());

	vec3 pos = m_ViewMatrix.GetTranslation();
	GetTransform()->SetPosition(pos);
	m_ViewMatrix.Inverse();

	m_pTransform->SetRotation(rot);
}

} // namespace fw
