#pragma once
#include "singleton.h"

class CLevel;

// ��� ���� ���� ������ ���� �Ŵ���
class CLevelMgr :
    public CSingleton<CLevelMgr>
{
    SINGLE(CLevelMgr);
private:
    CLevel* m_CurLevel;

public:
    void init();
    void tick();
    void render();

public:
    CLevel* GetCurrentLevel() { return m_CurLevel; }
};

