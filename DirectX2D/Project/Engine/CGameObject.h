#pragma once
#include "CEntity.h"

class CComponent;

class CGameObject :
    public CEntity
{
private:
    // 이제 모든 오브젝트는 컴포넌트를 배열로 들고 있는다.
    CComponent* m_arrCom[(UINT)COMPONENT_TYPE::END]; 

public:
    CGameObject();
    ~CGameObject();
};

