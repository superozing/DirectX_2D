#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

CLayer::CLayer()
	:m_iLayerIdx(NOT_SELECTED)
{

}

CLayer::~CLayer()
{
	Delete_Vec(m_vecParent);

}

void CLayer::begin()
{
	for (size_t i = 0; i < m_vecParent.size(); ++i)
	{
		m_vecParent[i]->begin();
	}
}

void CLayer::tick()
{
	for (size_t i = 0; i < m_vecParent.size(); ++i)
	{
		m_vecParent[i]->tick();
	}
}

void CLayer::finaltick()
{
	for (size_t i = 0; i < m_vecParent.size(); ++i)
	{
		m_vecParent[i]->finaltick();
	}
}

void CLayer::render()
{
	for (size_t i = 0; i < m_vecObjects.size(); ++i)
	{
		// �ڽ� ������ ��� ������Ʈ���� render ȣ��
		m_vecObjects[i]->render();
	}
}


void CLayer::AddObject(CGameObject* _Object, bool _bMove)
{
	// _bMove : true  - ���̾ �ԷµǴ� Object�� �ڽ��� �ִ� ���, �ڽı��� ��� �ش� ���̾�� �Ѿ�´�.
	// _bMove : false - ���̾ �ԷµǴ� Object�� �ڽ��� �ش� ���̾�� ���� �Ѿ���� �ʴ´�. �� �ڽĿ�����Ʈ�� ���̾� �Ҽ��� ���� ���(-1, NOT_SELECTED)���� ���� �����Ѵ�.

	// �ֻ��� �θ� ������Ʈ�� ���(�θ� ������Ʈ�� ���� ���)
	if (nullptr == _Object->GetParent())
	{
		// ���� �ٸ� ���̾� �Ҽ��� ������Ʈ�� ���, ���� ���̾��� Parent ���Ϳ��� �����ְ� push_back() ���ֱ�
		if (NOT_SELECTED != _Object->m_iLayerIdx)
			_Object->DisconnectWithLayer(); // ���� ���̾��� Parent ���Ϳ��� ����

		m_vecParent.push_back(_Object); // �ֻ��� �θ� ������Ʈ�� m_vecParent ���� ����Ű���� �Ѵ�.
	}

	// ������Ʈ�� ���̾� �Ҽ��� ���� ���̾�� ����
	// �ڽ� ������Ʈ���� ���̾� �Ҽ��� ����
	list<CGameObject*> queue;
	queue.push_back(_Object);

	// ���̾ �ԷµǴ� ������Ʈ ����, �� �ؿ� �޸� �ڽĵ���� ��� Ȯ��
	while (!queue.empty())
	{
		CGameObject* pObject = queue.front();
		queue.pop_front();

		for (size_t i = 0; i < pObject->m_vecChild.size(); ++i)
			queue.push_back(pObject->m_vecChild[i]);

		// ���� �Է� ������Ʈ�� ������ �ش� ���̾� �Ҽ����� ����
		if (pObject == _Object)
			pObject->m_iLayerIdx = m_iLayerIdx;
		else
		{
			if (_bMove)										// �ڽ� ������Ʈ���� _bMove �� true �� ���, �θ� ���� �ش� ���̾�Ҽ����� ����
				pObject->m_iLayerIdx = m_iLayerIdx;
			else if (NOT_SELECTED == pObject->m_iLayerIdx)	// _bMove �� false ��쿡��, �ڽ� ������Ʈ�� ���̾�Ҽ��� ���ٸ� �θ� ���� ����
				pObject->m_iLayerIdx = m_iLayerIdx;
		}
	}
}



void CLayer::DetachGameObject(CGameObject* _Object)
{
	// ������Ʈ�� ���̾ NOT_SELECTED�̰ų� ���� ���̾ �߸��� �ε����� ������ ���
	assert(!(_Object->m_iLayerIdx != m_iLayerIdx));

	
	if (nullptr != _Object->GetParent()) // �θ� ������Ʈ�� �ִ� ���
	{
		_Object->m_iLayerIdx = NOT_SELECTED;
	}
	else	// �ֻ��� �θ� ������Ʈ�� ���
	{
		// ���� �θ� ���̾�� ������Ʈ�� �����, NOT_SELECTED ���·� ����
		for (auto iter = m_vecParent.begin(); iter != m_vecParent.end(); ++iter)
		{
			if (*iter == _Object)
			{
				m_vecParent.erase(iter);
				_Object->m_iLayerIdx = NOT_SELECTED;
				return;
			}
		}
	}

	assert(nullptr);
}