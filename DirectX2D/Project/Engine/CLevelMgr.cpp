#include "pch.h"
#include "CLevelMgr.h"

#include "CDevice.h"
#include "CAssetMgr.h"

#include "CLevel.h"
#include "CGameObject.h"
#include "components.h"
#include "CPlayerScript.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
{

}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_CurLevel)
		delete m_CurLevel;
}

void CLevelMgr::init()
{
	// 초기 레벨 구성하기
	m_CurLevel = new CLevel;

	// GameObject 생성
	CGameObject* pObj = nullptr;

	pObj = new CGameObject;
	pObj->SetName(L"Player");

	// 오브젝트에 컴포넌트, 렌더컴포넌트, 스크립트 추가
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CPlayerScript);

	// Transform 정보 설정
	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

	// 매쉬, 쉐이더 설정
	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"Std2DShader"));

	// 레벨에 추가
	m_CurLevel->AddObject(pObj, 0);
}

void CLevelMgr::tick()
{
	if (nullptr == m_CurLevel)
		return;

	m_CurLevel->tick();
	m_CurLevel->finaltick();
}

void CLevelMgr::render()
{
	if (nullptr == m_CurLevel)
		return;

	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	CDevice::GetInst()->ClearView(ClearColor);

	// 현재 레벨의 물체 render
	m_CurLevel->render();


	CDevice::GetInst()->Present();
}
