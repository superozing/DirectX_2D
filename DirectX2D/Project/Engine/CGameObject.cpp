#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CRenderComponent.h"

#include "CScript.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

CGameObject::CGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_iLayerIdx(NOT_SELECTED) // -1 -> 레이어가 지정되지 않은 상태로 본다.
{
}

CGameObject::~CGameObject()
{
	// 컴포넌트와 스크립트 삭제 이후, 자식 삭제

	Delete_Array(m_arrCom);
	Delete_Vec(m_vecScript);
	Delete_Vec(m_vecChild);
}

void CGameObject::begin()
{
	// 자신의 begin()을 끝낸 이후 자신의 모든 자식 오브젝트들에게 begin 호출
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->begin();
		}
	}

	// child begin
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->begin();
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

	// child tick
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->tick();
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

	// 자신을 레이어에 등록 시킴
	CLayer* pCurLayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(m_iLayerIdx);
	pCurLayer->RegisterGameObject(this);

	// child finaltick
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finaltick();
	}
}

void CGameObject::render()
{
	// render component에게만 render를 호출
	if (nullptr != m_RenderCom)
	{
		m_RenderCom->render();
	}

	// child render
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->render();
	}
}

void CGameObject::DisconnectWithParent()
{
	// 부모로부터 부모의 자식 벡터(자신이 포함되어 있는 벡터)를 가져온다.
	for (auto iter = m_Parent->m_vecChild.begin(); iter != m_Parent->m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			// 부모의 자식 벡터에서 자신을 삭제, 자신의 부모를 nullptr로 만들어서 연결 끊기
			m_Parent->m_vecChild.erase(iter);
			m_Parent = nullptr;

			return;
		}
	}

	// 부모가 없는 오브젝트에 DisconnectWithParent 함수를 호출 했거나
	// 부모는 자식을 가리키기지 않고 있는데, 자식은 부모를 가리키고 있는 경우
	assert(nullptr);
}

void CGameObject::DisconnectWithLayer()
{
	// 이미 레이어 소속이 아닐 경우 바로 return
	if (NOT_SELECTED == m_iLayerIdx)
		return;

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pCurLayer = pCurLevel->GetLayer(m_iLayerIdx);
	pCurLayer->DetachGameObject(this);
}

void CGameObject::AddChild(CGameObject* _Child)
{
	// 이전 부모 오브젝트가 있을 경우 ->
	if (_Child->m_Parent)
	{
		// 이전 부모 오브젝트랑 연결 해제
		_Child->DisconnectWithParent();
	}

	// 부모 자식 서로 연결
	_Child->m_Parent = this;
	m_vecChild.push_back(_Child);
}

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
		if (nullptr != m_arrCom[(UINT)type])
		{
			MessageBox(nullptr, L"GameObject에 동일한 component를 2개 이상 넣으려고 함", L"[CGameObject.cpp]", MB_OK);
			assert(nullptr);
		}
		m_arrCom[(UINT)type] = _Component;
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
