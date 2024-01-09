#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer
	: public CEntity
{
private:
	// �θ� ������Ʈ�� (���� ����)
	vector<CGameObject*>	m_vecParent;

	// �θ� ������Ʈ�� �ڽ� ������Ʈ ���
	vector<CGameObject*>    m_vecObjects;

	int                     m_iLayerIdx;

private:
	void begin();
	void tick();
	void finaltick();
	void render();

	void AddObject(CGameObject* _Object, bool _bMove);

public:
	// Ư�� ������Ʈ�� ���̾�� ����
	void DetachGameObject(CGameObject* _Object);
	void RegisterGameObject(CGameObject* _Object) { m_vecObjects.push_back(_Object); }

public:
	CLayer();
	~CLayer();

	friend class CLevel; // Layer�� AddObject�� Layer�� �����ϴ� Level������ ȣ�� �����ϵ���
};

