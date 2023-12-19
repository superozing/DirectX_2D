#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"

// �Ŵ���
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

	// â ũ�� ����
	RECT winRect = { 0, 0, (LONG)m_vResolution.x, (LONG)m_vResolution.y };

	// â �ʱ�ȭ
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, false); // �޴� ��� ����
	SetWindowPos(m_hMainWindow, nullptr, 10, 10, (winRect.right - winRect.left), (winRect.bottom - winRect.top), 0);

	// ����̽� �ʱ�ȭ
	IF_FAILED(INST(CDevice)->init(m_hMainWindow, m_vResolution), L"����̽� �ʱ�ȭ");

	// �Ŵ��� �ʱ�ȭ
	CPathMgr::init();
	INST(CTimeMgr)->init();
	INST(CKeyMgr)->init();
	INST(CAssetMgr)->init();

	// Test �ʱ�ȭ
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
