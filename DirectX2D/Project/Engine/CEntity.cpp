#include "pch.h"
#include "CEntity.h"

// static 멤버 정의
UINT CEntity::g_NextID = 0;


CEntity::CEntity()
	: m_ID(g_NextID++) // ID를 입력하고 g_NextID++
{
}

CEntity::~CEntity()
{
}
