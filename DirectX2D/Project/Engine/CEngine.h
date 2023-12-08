#pragma once
#include "singleton.h"

class CEngine :
    public CSingleton<CEngine>
{
    SINGLE(CEngine);

private:
    HWND m_hMainWindow; // 메인 윈도우 핸들
    Vec2 m_vResolution; // 창의 해상도 정보


public: // Get, Set
    HWND GetMainWindow() const { return m_hMainWindow; }

public:
    int  init(HWND _hWnd, Vec2 vResolution);
    void progress(); // 기존 tick()
};

