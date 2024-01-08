#pragma once
#include "CEntity.h"

// ��ũ�� - ## �ڿ� ���ڷ� ġȯ�� ������. �߰��� class �� ����ؼ� ���漱��
#define GET_COMPONENT(Type, TYPE) class C##Type* Type() { return (C##Type*)m_arrCom[(UINT)COMPONENT_TYPE::##TYPE]; }


class CComponent;
class CRenderComponent;

class CScript;
class CCamera;

class CGameObject :
    public CEntity
{
private:
    // ���� ������Ʈ�� ������Ʈ�� �迭�� ����
    CComponent*     m_arrCom[(UINT)COMPONENT_TYPE::END];

    // render ���� ������Ʈ�� �� �� ���� ������ �� �ִ�.
    CRenderComponent* m_RenderCom;

    // �ϳ��� ���� ������Ʈ�� ���� ���� ��ũ��Ʈ�� ���� �� �ִ�.
    vector<CScript*> m_vecScript;

public:
    void begin();
    void tick();
    void finaltick();
    void render();

public:
    CComponent* GetComponent(COMPONENT_TYPE _Type) const { return m_arrCom[(UINT)_Type]; }

    // ��ũ�� ������ ����� �Լ� ����
    GET_COMPONENT(Transform, TRANSFORM);    // Transform ��° �ε����� ������Ʈ �迭���� ���� ����ȯ �� ��ȯ���ִ� �Լ��� �ȴ�
    GET_COMPONENT(MeshRender, MESHRENDER);
    GET_COMPONENT(Camera, CAMERA);



public:
    void AddComponent(CComponent* _Comonent);


public:
    CGameObject();
    ~CGameObject();
};

