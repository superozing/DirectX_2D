#pragma once
#include "CComponent.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
//#include "CResMgr.h" // 나중에 추가될 리소스 매니저

// 연관이 많기 때문에 전방 선언만 하는 것이 아닌 include를 해준다.
#include "CGameObject.h"
#include "components.h"

// 유니티 방식을 따른 Script 구조 설계하자.
class CScript :
    public CComponent

{
private:
    UINT m_iScriptType;

public:
    virtual void finaltick() final {} // 스크립트를 상속받은 실제로 사용할 스크립트에서 finaltick()을 정의하지 못하도록 막아놓기

public:
    CScript();
    ~CScript();
};

