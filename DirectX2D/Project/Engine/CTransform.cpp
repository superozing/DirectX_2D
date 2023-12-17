#include "pch.h"
#include "CTransform.h"

CTransform::CTransform()
	:CComponent(COMPONENT_TYPE::TRANSFORM) // ������Ʈ�� �ڽ� Ÿ�� ����
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
}

