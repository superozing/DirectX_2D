#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"
#include "CTransform.h"

CCamera::CCamera()
		: CComponent(COMPONENT_TYPE::CAMERA)
		, m_ProjType(PROJ_TYPE::PERSPECTIVE)
		, m_FOV(XM_PI / 2.f)
		, m_Width(0.f)
		, m_Scale(1.f)
		, m_AspectRatio(1.f)
		, m_Far(10000.f)
{
	Vec2 vResol = INST(CDevice)->GetRenderResolution();
	m_AspectRatio = vResol.x / vResol.y;
}

CCamera::~CCamera()
{
}

void CCamera::finaltick()
{
	// 뷰 행렬 계산
	Vec3 vCamPos = Transform()->GetRelativePos();

	m_matView = XMMatrixIdentity();

	// 카메라가 바라보는 방향을 뺴주어야 함.
	// 카메라가 움직이는 것이 아닌, 모든 물체가 카메라의 반대 방향으로 이동하는 것.
	m_matView._41 = -vCamPos.x;
	m_matView._42 = -vCamPos.y;
	m_matView._43 = -vCamPos.z;

	// 투영 방식에 따른 투영 행렬 계산
	m_matProj = XMMatrixIdentity();

	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{
		// 직교투영
		Vec2 vResol = INST(CDevice)->GetRenderResolution();
		m_matProj = XMMatrixOrthographicLH(vResol.x * m_Scale, (vResol.x / m_AspectRatio) * m_Scale, 1.f, m_Far);
	}
	else
	{
		// 원근투영
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}

	// 계산한 view 행렬과 proj 행렬을 전역변수에 담아둔다.
	g_Transform.matView = m_matView;
	g_Transform.matProj = m_matProj;
}
