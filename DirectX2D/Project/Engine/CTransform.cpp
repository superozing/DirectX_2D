#include "pch.h"
#include "CTransform.h"
#include "CConstBuffer.h"
#include "CDevice.h"

CTransform::CTransform()
	:CComponent(COMPONENT_TYPE::TRANSFORM) // 컴포넌트에 자신 타입 명시
{
}

CTransform::~CTransform()
{
}

void CTransform::finaltick()
{
	UpdateData();
}

void CTransform::UpdateData()
{
	tTransform transform = {};
	transform.vWorldPos = m_vRelativePos;
	transform.vWorldScale = m_vRelativeScale;

	// 상수 버퍼 추가 후 구현 예정
	// 위치정보를 Transform 상수버퍼에 보내고, B0 레지스터에 바인딩 해둠
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pCB->SetData(&transform);
	pCB->UpdateData(0);
}

