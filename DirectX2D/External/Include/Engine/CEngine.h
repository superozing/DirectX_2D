#pragma once
#include "singleton.h"

class CEngine :
    public CSingleton<CEngine>
{
    SINGLE(CEngine);

private:
    HWND m_hMainWindow; // ���� ������ �ڵ�
    Vec2 m_vResolution; // â�� �ػ� ����


public: // Get, Set
    HWND GetMainWindow() const { return m_hMainWindow; }

public:
    int  init(HWND _hWnd, Vec2 vResolution);
    void progress(); // ���� tick()
};

