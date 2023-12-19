#pragma once
#include "CEntity.h"

class CGameObject;

class CComponent :
    public CEntity
{
private:
    const COMPONENT_TYPE    m_Type;     // ������Ʈ Ÿ���� ������ ������ ���߿� dynamic_cast�� ������� �ʾƵ� �ٷ� ���� ����ȯ�� �����ϴ�.
    CGameObject*            m_Owner;    // �ش� ������Ʈ�� �����ϴ� ���ӿ�����Ʈ ������. �ֹ����� ����Ŵ


public:
    virtual void begin() {}             // ���� ���� �Լ��� �ƴ����� ��ӹ޾� ����� �� �ֵ��� ����� ����.
    virtual void tick() {}
    virtual void finaltick() = 0; // finaltick()�� ���� fix �۾�. ������Ʈ ���� �ؾ� �� ���� ���� finaltick()�� ���� ����
    virtual void UpdateData() {}  // GPU ���۷� ������ ������ �� �ǹ��ϴ� �Լ�

public:
    COMPONENT_TYPE GetType() const { return m_Type; }
    CGameObject* GetOwner() const { return m_Owner; }

public:
    CComponent(COMPONENT_TYPE _type);
    ~CComponent();

    friend class CGameObject;
};

