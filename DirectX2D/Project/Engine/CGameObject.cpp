#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CRenderComponent.h"

#include "CScript.h"

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
	// component tick
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->tick();
		}
	}

	// script tick
	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->tick();
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
	// render component에게만 render를 호출
	if (nullptr != m_RenderCom)
	{
		m_RenderCom->render();
	}
}

#define TYPE (UINT)type
void CGameObject::AddComponent(CComponent* _Component)
{
	COMPONENT_TYPE type = _Component->GetType();

	// Script일 경우
	if (type == COMPONENT_TYPE::SCRIPT)
	{
		// 만약 Script가 아닐 경우에 대한 예외 처리
		assert(dynamic_cast<CScript*>(_Component));

		// Script 벡터에 추가
		m_vecScript.push_back((CScript*)_Component);
		_Component->m_Owner = this;

	}
	// Script가 아닌 기본 컴포넌트일 경우
	else
	{
		if (nullptr != m_arrCom[TYPE])
		{
			MessageBox(nullptr, L"GameObject에 동일한 component를 2개 이상 넣으려고 함", L"[CGameObject.cpp]", MB_OK);
			assert(nullptr);
		}
		m_arrCom[TYPE] = _Component;
		_Component->m_Owner = this;
		

		// 이 컴포넌트는 render component인가?
		CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(_Component);
		if (nullptr != pRenderCom)
		{
			// 이미 한 종류 이상의 RenderComponent 를 보유하고 있는 경우
			if (nullptr != m_RenderCom)
			{
				MessageBox(nullptr, L"GameObject에 rendercomponent를 2개 이상 넣으려고 함", L"[CGameObject.cpp]", MB_OK);
				assert(nullptr);
			}
			m_RenderCom = pRenderCom;
		}

	}

}
#undef TYPE