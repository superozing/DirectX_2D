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
		// 자신 레벨의 모든 오브젝트에게 render 호출
		m_vecObjects[i]->render();
	}
}


void CLayer::AddObject(CGameObject* _Object, bool _bMove)
{
	// _bMove : true  - 레이어에 입력되는 Object가 자식이 있는 경우, 자식까지 모두 해당 레이어로 넘어온다.
	// _bMove : false - 레이어에 입력되는 Object의 자식은 해당 레이어로 같이 넘어오지 않는다. 단 자식오브젝트가 레이어 소속이 없는 경우(-1, NOT_SELECTED)에만 같이 변경한다.

	// 최상위 부모 오브젝트인 경우(부모 오브젝트가 없을 경우)
	if (nullptr == _Object->GetParent())
	{
		// 만약 다른 레이어 소속의 오브젝트일 경우, 기존 레이어의 Parent 벡터에서 없애주고 push_back() 해주기
		if (NOT_SELECTED != _Object->m_iLayerIdx)
			_Object->DisconnectWithLayer(); // 기존 레이어의 Parent 벡터에서 제거

		m_vecParent.push_back(_Object); // 최상위 부모 오브젝트는 m_vecParent 에서 가리키도록 한다.
	}

	// 오브젝트의 레이어 소속을 현재 레이어로 변경
	// 자식 오브젝트들의 레이어 소속을 변경
	list<CGameObject*> queue;
	queue.push_back(_Object);

	// 레이어에 입력되는 오브젝트 포함, 그 밑에 달린 자식들까지 모두 확인
	while (!queue.empty())
	{
		CGameObject* pObject = queue.front();
		queue.pop_front();

		for (size_t i = 0; i < pObject->m_vecChild.size(); ++i)
			queue.push_back(pObject->m_vecChild[i]);

		// 최초 입력 오브젝트는 무조건 해당 레이어 소속으로 변경
		if (pObject == _Object)
			pObject->m_iLayerIdx = m_iLayerIdx;
		else
		{
			if (_bMove)										// 자식 오브젝트들은 _bMove 가 true 인 경우, 부모를 따라서 해당 레이어소속으로 변경
				pObject->m_iLayerIdx = m_iLayerIdx;
			else if (NOT_SELECTED == pObject->m_iLayerIdx)	// _bMove 가 false 경우에도, 자식 오브젝트의 레이어소속이 없다면 부모를 따라서 변경
				pObject->m_iLayerIdx = m_iLayerIdx;
		}
	}
}



void CLayer::DetachGameObject(CGameObject* _Object)
{
	// 오브젝트의 레이어가 NOT_SELECTED이거나 현재 레이어에 잘못된 인덱스가 들어왔을 경우
	assert(!(_Object->m_iLayerIdx != m_iLayerIdx));

	
	if (nullptr != _Object->GetParent()) // 부모 오브젝트가 있는 경우
	{
		_Object->m_iLayerIdx = NOT_SELECTED;
	}
	else	// 최상위 부모 오브젝트인 경우
	{
		// 현재 부모 레이어에서 오브젝트를 지우고, NOT_SELECTED 상태로 만듦
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