#include "pch.h"
#include "CComponent.h"



// �⺻ �����ڸ� �������� ����. �ݵ�� ������Ʈ Ÿ���� ���ڷ� �޵���
CComponent::CComponent(COMPONENT_TYPE _type)
	:m_Type(_type)
	,m_Owner(nullptr)
{
}

CComponent::~CComponent()
{
}
