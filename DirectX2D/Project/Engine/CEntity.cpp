#include "pch.h"
#include "CEntity.h"

// static ��� ����
UINT CEntity::g_NextID = 0;


CEntity::CEntity()
	: m_ID(g_NextID++) // ID�� �Է��ϰ� g_NextID++
{
}

CEntity::~CEntity()
{
}
