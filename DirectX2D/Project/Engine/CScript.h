#pragma once
#include "CComponent.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
//#include "CResMgr.h" // ���߿� �߰��� ���ҽ� �Ŵ���

// ������ ���� ������ ���� ���� �ϴ� ���� �ƴ� include�� ���ش�.
#include "CGameObject.h"
#include "components.h"

// ����Ƽ ����� ���� Script ���� ��������.
class CScript :
    public CComponent

{
private:
    UINT m_iScriptType;

public:
    virtual void finaltick() final {} // ��ũ��Ʈ�� ��ӹ��� ������ ����� ��ũ��Ʈ���� finaltick()�� �������� ���ϵ��� ���Ƴ���

public:
    CScript();
    ~CScript();
};

