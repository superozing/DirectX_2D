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
	, m_iLayerIdx(NOT_SELECTED) // -1 -> ���̾ �������� ���� ���·� ����.
{
}

CGameObject::~CGameObject()
{
	// ������Ʈ�� ��ũ��Ʈ ���� ����, �ڽ� ����

	Delete_Array(m_arrCom);
	Delete_Vec(m_vecScript);
	Delete_Vec(m_vecChild);
}

void CGameObject::begin()
{
	// �ڽ��� begin()�� ���� ���� �ڽ��� ��� �ڽ� ������Ʈ�鿡�� begin ȣ��
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

	// �ڽ��� ���̾ ��� ��Ŵ
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
	// render component���Ը� render�� ȣ��
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
	// �θ�κ��� �θ��� �ڽ� ����(�ڽ��� ���ԵǾ� �ִ� ����)�� �����´�.
	for (auto iter = m_Parent->m_vecChild.begin(); iter != m_Parent->m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			// �θ��� �ڽ� ���Ϳ��� �ڽ��� ����, �ڽ��� �θ� nullptr�� ���� ���� ����
			m_Parent->m_vecChild.erase(iter);
			m_Parent = nullptr;

			return;
		}
	}

	// �θ� ���� ������Ʈ�� DisconnectWithParent �Լ��� ȣ�� �߰ų�
	// �θ�� �ڽ��� ����Ű���� �ʰ� �ִµ�, �ڽ��� �θ� ����Ű�� �ִ� ���
	assert(nullptr);
}

void CGameObject::DisconnectWithLayer()
{
	// �̹� ���̾� �Ҽ��� �ƴ� ��� �ٷ� return
	if (NOT_SELECTED == m_iLayerIdx)
		return;

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pCurLayer = pCurLevel->GetLayer(m_iLayerIdx);
	pCurLayer->DetachGameObject(this);
}

void CGameObject::AddChild(CGameObject* _Child)
{
	// ���� �θ� ������Ʈ�� ���� ��� ->
	if (_Child->m_Parent)
	{
		// ���� �θ� ������Ʈ�� ���� ����
		_Child->DisconnectWithParent();
	}

	// �θ� �ڽ� ���� ����
	_Child->m_Parent = this;
	m_vecChild.push_back(_Child);
}

void CGameObject::AddComponent(CComponent* _Component)
{
	COMPONENT_TYPE type = _Component->GetType();

	// Script�� ���
	if (type == COMPONENT_TYPE::SCRIPT)
	{
		// ���� Script�� �ƴ� ��쿡 ���� ���� ó��
		assert(dynamic_cast<CScript*>(_Component));

		// Script ���Ϳ� �߰�
		m_vecScript.push_back((CScript*)_Component);
		_Component->m_Owner = this;

	}
	// Script�� �ƴ� �⺻ ������Ʈ�� ���
	else
	{
		if (nullptr != m_arrCom[(UINT)type])
		{
			MessageBox(nullptr, L"GameObject�� ������ component�� 2�� �̻� �������� ��", L"[CGameObject.cpp]", MB_OK);
			assert(nullptr);
		}
		m_arrCom[(UINT)type] = _Component;
		_Component->m_Owner = this;
		

		// �� ������Ʈ�� render component�ΰ�?
		CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(_Component);
		if (nullptr != pRenderCom)
		{
			// �̹� �� ���� �̻��� RenderComponent �� �����ϰ� �ִ� ���
			if (nullptr != m_RenderCom)
			{
				MessageBox(nullptr, L"GameObject�� rendercomponent�� 2�� �̻� �������� ��", L"[CGameObject.cpp]", MB_OK);
				assert(nullptr);
			}
			m_RenderCom = pRenderCom;
		}

	}

}
