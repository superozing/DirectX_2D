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

	// ũ�� ����, ȸ�� ����, ��ġ ���� ��� �� ���� ��Ŀ� matScale ��� ���ϱ�/
	// �Լ��� ���ϰ� ���� �� ����.
	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

	Matrix matRotX = XMMatrixRotationX(m_vRealtiveRotation.x);
	Matrix matRotY = XMMatrixRotationY(m_vRealtiveRotation.y);
	Matrix matRotZ = XMMatrixRotationZ(m_vRealtiveRotation.z);

	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	m_matWorld = matScale * matRotX * matRotY * matRotZ * matTranslation;

	// ��ü�� ���Ⱚ�� �ٽ� ����Ѵ�.
	m_arrDir[(UINT)DIR_TYPE::RIGHT] = Vec3(1.f, 0.f, 0.f);
	m_arrDir[(UINT)DIR_TYPE::UP] = Vec3(0.f, 1.f, 0.f);
	m_arrDir[(UINT)DIR_TYPE::FRONT] = Vec3(0.f, 0.f, 1.f);

	// Vec3 �� Vec4 Ÿ������ Ȯ���ؼ� ����� ������Ѿ� ��
	// XMVector3TransformCoord	- w �� 1�� Ȯ��
	// XMVector3TransformNormal - w �� 0���� Ȯ��
	// mul(float4(_in.vPos, 1 or 0), g_matWorld); 
	// ���� ���� ��������� �̵��� �������� ���� ����
	for (int i = 0; i < 3; ++i)
	{
		// m_matWorld ��Ŀ� ũ�������� ���� �� �ֱ� ������ �ٽ� ���̸� 1�� ����ȭ ��Ų��.
		m_arrDir[i] = XMVector3TransformNormal(m_arrDir[i], m_matWorld);
		m_arrDir[i].Normalize();
	}
}

void CTransform::UpdateData()
{
	g_Transform.matWorld = m_matWorld;

	g_Transform.matWV = g_Transform.matWorld * g_Transform.matView;
	g_Transform.matWVP = g_Transform.matWV * g_Transform.matProj;

	// ��ġ������ Transform ������ۿ� ������, B0 �������Ϳ� ���ε� �ص�
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pCB->SetData(&g_Transform);
	pCB->UpdateData(0);
}