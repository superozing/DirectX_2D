#pragma once
#include "CEntity.h"

class CComponent;

class CGameObject :
    public CEntity
{
private:
    // ���� ��� ������Ʈ�� ������Ʈ�� �迭�� ��� �ִ´�.
    CComponent* m_arrCom[(UINT)COMPONENT_TYPE::END]; 

public:
    CGameObject();
    ~CGameObject();
};

