#pragma once
#include "CEntity.h"

class CComponent;

class CGameObject :
    public CEntity
{
private:
    // 게임 오브젝트는 컴포넌트를 
    CComponent*     m_arrCom[(UINT)COMPONENT_TYPE::END];


public:
    void begin();

    void tick();

    void finaltick();

    void render();



public:
    void AddComponent(CComponent* _Comonent);


public:
    CGameObject();
    ~CGameObject();
};

