#include "pch.h"
#include "Test.h"

#include "CDevice.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

//#include "CMesh.h"
//#include "CGraphicsShader.h"

tTransform g_Transform = { Vec4(0.f, 0.f, 0.f, 0.f), Vec4(1.f, 1.f, 1.f, 1.f) };






// ��� ���� (Ʈ������ ���� �ִ�)
ComPtr<ID3D11Buffer> g_CB = nullptr;

int TestInit()
{
	return 0;
}

void TestProgress()
{
}

void TestRelease()
{
}
