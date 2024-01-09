#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;


// ��� ������ ����� �� Ŭ���� CLevel
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
	void clear(); // ���� �Ŵ��������� clear�� �� �� �ֵ��� ����� ó�� �� friendó��

public:
	CLevel();
	~CLevel();

	friend class CLevelMgr;
};
