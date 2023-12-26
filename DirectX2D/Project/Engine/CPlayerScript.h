#pragma once
#include "CScript.h"

// 키 입력에 따라 플레이어 움직임 기능
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

