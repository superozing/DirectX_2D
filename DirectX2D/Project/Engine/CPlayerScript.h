#pragma once
#include "CScript.h"

// Ű �Է¿� ���� �÷��̾� ������ ���
class CPlayerScript :
    public CScript
{
private:
    float m_Speed;

public:
    virtual void tick() override;

public:
    CPlayerScript();
    ~CPlayerScript();
};

