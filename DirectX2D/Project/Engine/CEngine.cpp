#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"

// 매니저
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CPathMgr.h"
#include "CAssetMgr.h"

#include "Test.h"

CEngine::CEngine()
	: m_hMainWindow(nullptr)
	, m_vResolution()
{
}

CEngine::~CEngine()
{
	TestRelease();
}

int CEngine::init(HWND _hWnd, Vec2 vResolution)
{
	m_hMainWindow = _hWnd;
	m_vResolution = vResolution;

	// 창 크기 설정
	RECT winRect = { 0, 0, (LONG)m_vResolution.x, (LONG)m_vResolution.y };

	// 창 초기화
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, false); // 메뉴 사용 안함
	SetWindowPos(m_hMainWindow, nullptr, 10, 10, (winRect.right - winRect.left), (winRect.bottom - winRect.top), 0);

	// 디바이스 초기화
	IF_FAILED(INST(CDevice)->init(m_hMainWindow, m_vResolution), L"디바이스 초기화");

	// 매니저 초기화
	CPathMgr::init();
	INST(CTimeMgr)->init();
	INST(CKeyMgr)->init();
	INST(CAssetMgr)->init();

	// Test 초기화
	IF_FAILED(TestInit(), L"TestInit");

	return S_OK;
}

void CEngine::progress()
{
	// Manager Update
	INST(CTimeMgr)->tick();
	INST(CKeyMgr)->tick();

	// Test Update
	TestProgress();
}
