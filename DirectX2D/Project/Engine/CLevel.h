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

	void AddObject(CGameObject* _Object, int _LayerIdx, bool _bChildMove = true);
	CLayer* GetLayer(int _iLayerIdx) const { return m_arrLayer[_iLayerIdx]; }

private:
	void clear(); // 레벨 매니저에서만 clear를 할 수 있도록 비공개 처리 후 friend처리

public:
	CLevel();
	~CLevel();

	friend class CLevelMgr;
};
