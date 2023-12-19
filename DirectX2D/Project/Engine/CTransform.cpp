#include "pch.h"
#include "CTransform.h"
#include "CConstBuffer.h"
#include "CDevice.h"

CTransform::CTransform()
	:CComponent(COMPONENT_TYPE::TRANSFORM) // ������Ʈ�� �ڽ� Ÿ�� ���
	, m_vRelativeScale(Vec3(1.f,1.f,1.f))  // ������Ʈ�� �⺻ Scale ������ 1��� ���. �⺻ ���� 0���� �����ϸ� ������ ����.
{
}

CTransform::~CTransform()
{
}

void CTransform::finaltick()
{
	// UpdateData()�� �� ƽ���� ȣ���ϸ� ���� �������� ȣ��Ǵ� UpdateData�� ��� ��ü�� ������������ �����ϰ� �ȴ�.
	// UpdateData()�� ���⼭ ȣ���ϴ� ���� �ƴ϶� render ������ ȣ�����־�� �Ѵ�.
}

void CTransform::UpdateData()
{
	tTransform transform = {};
	transform.vWorldPos = m_vRelativePos;
	transform.vWorldScale = m_vRelativeScale;

	// ��ġ������ Transform ������ۿ� ������, B0 �������Ϳ� ���ε� �ص�
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pCB->SetData(&transform);
	pCB->UpdateData(0);
}

