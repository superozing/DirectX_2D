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


	// �������(nxn ���Ұ� 1�� ���)
	m_matWorld = XMMatrixIdentity();

	// nxn ���ҿ� scale �߰�
	m_matWorld._11 = m_vRelativeScale.x;
	m_matWorld._22 = m_vRelativeScale.y;
	m_matWorld._33 = m_vRelativeScale.z;

	// ������ �࿡ pos �߰�
	m_matWorld._41 = m_vRelativePos.x;
	m_matWorld._42 = m_vRelativePos.y;
	m_matWorld._43 = m_vRelativePos.z;
}

void CTransform::UpdateData()
{
	tTransform transform = {};
	transform.matWorld = m_matWorld;

	// ��ġ������ Transform ������ۿ� ������, B0 �������Ϳ� ���ε� �ص�
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pCB->SetData(&transform);
	pCB->UpdateData(0);
}

