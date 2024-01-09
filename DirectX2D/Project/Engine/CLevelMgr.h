#pragma once
#include "singleton.h"

class CLevel;

// 모든 레벨 들을 관리할 레벨 매니저
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

