#include "pch.h"
#include "CEngine.h"


CEngine::CEngine()
	: m_hMainWindow(nullptr)
	, m_vResolution()
{
}

CEngine::~CEngine()
{
	// Test Release
}

int CEngine::init(HWND _hWnd, Vec2 vResolution)
{
	m_hMainWindow = _hWnd;
	m_vResolution = vResolution;

	RECT winRect = { 0, 0, (LONG)m_vResolution.x, (LONG)m_vResolution.y };

	// 창 초기화
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, false); // 메뉴 사용 안함
	SetWindowPos(m_hMainWindow, nullptr
				, 10, 10
				, (winRect.right - winRect.left)
				, (winRect.bottom - winRect.top)
				, 0);

	// 매니저 초기화



	// Test 초기화


	return 0;
}

void CEngine::progress()
{
}
