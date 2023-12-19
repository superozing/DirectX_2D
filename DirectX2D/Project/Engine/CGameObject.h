#pragma once
#include "CEntity.h"

// 매크로 - ## 뒤에 문자로 치환이 가능함. 중간에 class 를 사용해서 전방선언
#define GET_COMPONENT(Type, TYPE) class C##Type* Type() { return (C##Type*)m_arrCom[(UINT)COMPONENT_TYPE::##TYPE]; }


class CComponent;
class CRenderComponent;

class CGameObject :
    public CEntity
{
private:
    // 게임 오브젝트는 컴포넌트를 배열로 관리
    CComponent*     m_arrCom[(UINT)COMPONENT_TYPE::END];

    // render 관련 컴포넌트는 단 한 개만 설정할 수 있다.
    CRenderComponent* m_RenderCom;

public:
    void begin();
    void tick();
    void finaltick();
    void render();

public:
    CComponent* GetComponent(COMPONENT_TYPE _Type) const { return m_arrCom[(UINT)_Type]; }

    // 매크로 구문을 사용한 함수 정의
    GET_COMPONENT(Transform, TRANSFORM);    // Transform 번째 인덱스를 컴포넌트 배열에서 꺼내 형변환 후 반환해주는 함수가 된다
    GET_COMPONENT(MeshRender, MESHRENDER);



public:
    void AddComponent(CComponent* _Comonent);


public:
    CGameObject();
    ~CGameObject();
};

