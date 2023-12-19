#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CRenderComponent.h"

CGameObject::CGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
{
}

CGameObject::~CGameObject()
{
	Delete_Array(m_arrCom);
}

void CGameObject::begin()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->begin();
		}
	}
}

void CGameObject::tick()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->tick();
		}
	}
}

void CGameObject::finaltick()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->finaltick();
		}
	}
}

void CGameObject::render()
{
	// render component에게만 render를 보냄.
	if (nullptr != m_RenderCom)
	{
		m_RenderCom->render();
	}
}

void CGameObject::AddComponent(CComponent* _Comonent)
{
	COMPONENT_TYPE type = _Comonent->GetType();

	m_arrCom[(UINT)type] = _Comonent;
	_Comonent->m_Owner = this;

	// 컴포넌트를 render component로 dynamic_cast 해보기. 만약 아닐 경우 nullptr이 들어가게 된다.
	m_RenderCom = dynamic_cast<CRenderComponent*>(_Comonent);
}