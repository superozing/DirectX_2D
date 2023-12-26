#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;


// 모든 레벨이 기반이 될 클래스 CLevel
class CLevel :
	public CEntity
{
private:
	CLayer* m_arrLayer[LAYER_MAX];


public:
	void begin();
	void tick();
	void finaltick();
	void render();

	void AddObject(CGameObject* _Object, int _LayerIdx);

public:
	CLevel();
	~CLevel();
};
