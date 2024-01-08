#include "pch.h"
#include "CCamera.h"

#include "CTransform.h"

CCamera::CCamera()
		: CComponent(COMPONENT_TYPE::CAMERA)
		, m_FOV(0.f)
		, m_Width(0.f)
		, m_Scale(0.f)
		, m_AspectRatio(1.f)
		, m_Far(10000.f)
{
}

CCamera::~CCamera()
{
}

void CCamera::finaltick()
{
	Vec3 vCamPos = Transform()->GetRelativePos();

	m_matView = XMMatrixIdentity();

	// 카메라가 바라보는 방향을 뺴주어야 함.
	// 카메라가 움직이는 것이 아닌, 모든 물체가 카메라의 반대 방향으로 이동하는 것.
	m_matView._41 = -vCamPos.x;
	m_matView._42 = -vCamPos.y;
	m_matView._43 = -vCamPos.z;


	m_matProj = XMMatrixIdentity();

	g_Transform.matView = m_matView;
	g_Transform.matProj = m_matProj;
}
