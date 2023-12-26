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
	// render component���Ը� render�� ȣ��
	if (nullptr != m_RenderCom)
	{
		m_RenderCom->render();
	}
}

#define TYPE (UINT)type
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
		if (nullptr != m_arrCom[TYPE])
		{
			MessageBox(nullptr, L"GameObject�� ������ component�� 2�� �̻� �������� ��", L"[CGameObject.cpp]", MB_OK);
			assert(nullptr);
		}
		m_arrCom[TYPE] = _Component;
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
#undef TYPE