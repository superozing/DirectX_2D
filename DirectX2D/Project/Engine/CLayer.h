#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer
	: public CEntity
{
private:
	vector<CGameObject*> m_vecParent;

private:
	void begin();
	void tick();
	void finaltick();
	void render();

public:
	CLayer();
	~CLayer();


private:
	void AddObject(CGameObject* _Object) { m_vecParent.push_back(_Object); }

	friend class CLevel; // Layer의 AddObject는 Layer를 소유하는 Level에서만 호출 가능하도록
};

