#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer
	: public CEntity
{
private:
	// 부모 오브젝트만 (계층 구조)
	vector<CGameObject*>	m_vecParent;

	// 부모 오브젝트와 자식 오브젝트 모두
	vector<CGameObject*>    m_vecObjects;

	int                     m_iLayerIdx;

private:
	void begin();
	void tick();
	void finaltick();
	void render();

	void AddObject(CGameObject* _Object, bool _bMove);

public:
	// 특정 오브젝트를 레이어에서 제거
	void DetachGameObject(CGameObject* _Object);
	void RegisterGameObject(CGameObject* _Object) { m_vecObjects.push_back(_Object); }

public:
	CLayer();
	~CLayer();

	friend class CLevel; // Layer의 AddObject는 Layer를 소유하는 Level에서만 호출 가능하도록
};

