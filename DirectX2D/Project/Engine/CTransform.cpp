#include "pch.h"
#include "CTransform.h"
#include "CConstBuffer.h"
#include "CDevice.h"

CTransform::CTransform()
	:CComponent(COMPONENT_TYPE::TRANSFORM) // 컴포넌트에 자신 타입 명시
	, m_vRelativeScale(Vec3(1.f,1.f,1.f))  // 컴포넌트의 기본 Scale 배율을 1배로 잡기. 기본 값인 0으로 설정하면 보이지 않음.
{
}

CTransform::~CTransform()
{
}

void CTransform::finaltick()
{
	// UpdateData()를 매 틱마다 호출하면 제일 마지막에 호출되는 UpdateData로 모든 물체의 파이프라인을 실행하게 된다.
	// UpdateData()는 여기서 호출하는 것이 아니라 render 직전에 호출해주어야 한다.


	// 단위행렬(nxn 원소가 1인 행렬)
	m_matWorld = XMMatrixIdentity();

	// 크기 정보, 회전 정보, 위치 정보 계산 후 월드 행렬에 matScale 행렬 곱하기/
	// 함수로 편하게 만들 수 있음.
	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

	Matrix matRotX = XMMatrixRotationX(m_vRealtiveRotation.x);
	Matrix matRotY = XMMatrixRotationY(m_vRealtiveRotation.y);
	Matrix matRotZ = XMMatrixRotationZ(m_vRealtiveRotation.z);

	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	m_matWorld = matScale * matRotX * matRotY * matRotZ * matTranslation;

	// 물체의 방향값을 다시 계산한다.
	m_arrDir[(UINT)DIR_TYPE::RIGHT] = Vec3(1.f, 0.f, 0.f);
	m_arrDir[(UINT)DIR_TYPE::UP] = Vec3(0.f, 1.f, 0.f);
	m_arrDir[(UINT)DIR_TYPE::FRONT] = Vec3(0.f, 0.f, 1.f);

	// Vec3 를 Vec4 타입으로 확장해서 행렬을 적용시켜야 함
	// XMVector3TransformCoord	- w 를 1로 확장
	// XMVector3TransformNormal - w 를 0으로 확장
	// mul(float4(_in.vPos, 1 or 0), g_matWorld); 
	// 적용 받을 상태행렬의 이동을 적용할지 말지 결정
	for (int i = 0; i < 3; ++i)
	{
		// m_matWorld 행렬에 크기정보가 있을 수 있기 때문에 다시 길이를 1로 정규화 시킨다.
		m_arrDir[i] = XMVector3TransformNormal(m_arrDir[i], m_matWorld);
		m_arrDir[i].Normalize();
	}
}

void CTransform::UpdateData()
{
	g_Transform.matWorld = m_matWorld;

	// 위치정보를 Transform 상수버퍼에 보내고, B0 레지스터에 바인딩 해둠
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pCB->SetData(&g_Transform);
	pCB->UpdateData(0);
}