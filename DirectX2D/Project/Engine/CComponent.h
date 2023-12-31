#pragma once
#include "CEntity.h"

#include "CGameObject.h"

// 컴포넌트 함수 생성 매크로
#define GET_OTHER_COMPONENT(Type) C##Type* Type() { return m_Owner->Type(); }


class CComponent :
    public CEntity
{
private:
    const COMPONENT_TYPE    m_Type;     // 컴포넌트 타입을 가지고 있으면 나중에 dynamic_cast를 사용하지 않아도 바로 강제 형변환이 가능하다.
    CGameObject*            m_Owner;    // 해당 컴포넌트를 소유하는 게임오브젝트 포인터. 쌍방으로 가리킴


public:
    virtual void begin() {}             // 순수 가상 함수는 아니지만 상속받아 사용할 수 있도록 만들기 위해.
    virtual void tick() {}
    virtual void finaltick() = 0; // finaltick()은 최종 fix 작업. 컴포넌트 들이 해야 할 일을 보통 finaltick()에 구현 예정
    virtual void UpdateData() {}  // GPU 버퍼로 데이터 보내기 를 의미하는 함수

public:
    COMPONENT_TYPE GetType() const { return m_Type; }
    CGameObject* GetOwner() { return m_Owner; }


public: // 매크로로 함수 생성
    GET_OTHER_COMPONENT(Transform);
    GET_OTHER_COMPONENT(MeshRender);
    GET_OTHER_COMPONENT(Camera);

public:
    CComponent(COMPONENT_TYPE _type);
    ~CComponent();

    friend class CGameObject;
};

