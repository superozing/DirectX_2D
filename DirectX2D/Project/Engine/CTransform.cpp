#include "pch.h"
#include "CTransform.h"
#include "CConstBuffer.h"
#include "CDevice.h"

CTransform::CTransform()
	:CComponent(COMPONENT_TYPE::TRANSFORM) // ������Ʈ�� �ڽ� Ÿ�� ���
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

	// ��� ���� �߰� �� ���� ����
	// ��ġ������ Transform ������ۿ� ������, B0 �������Ϳ� ���ε� �ص�
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pCB->SetData(&transform);
	pCB->UpdateData(0);
}

