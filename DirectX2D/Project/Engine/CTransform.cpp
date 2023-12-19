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
}

void CTransform::UpdateData()
{
	tTransform transform = {};
	transform.vWorldPos = m_vRelativePos;
	transform.vWorldScale = m_vRelativeScale;

	// 위치정보를 Transform 상수버퍼에 보내고, B0 레지스터에 바인딩 해둠
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pCB->SetData(&transform);
	pCB->UpdateData(0);
}

