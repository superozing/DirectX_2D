#include "pch.h"
#include "CLevelMgr.h"

#include "CDevice.h"
#include "CAssetMgr.h"

#include "CLevel.h"
#include "CGameObject.h"
#include "components.h"
#include "CPlayerScript.h"
#include "CCameraMoveScript.h"

#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CTexture.h"

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

	// 텍스쳐 생성
	CTexture* pTex = INST(CAssetMgr)->Load<CTexture>(L"PlayerTexture", L"texture\\Character.png");
	if (nullptr != pTex)
		pTex->UpdateData(0);
	
	// Camera Object 생성
	CGameObject* pCamObj = new CGameObject;
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	m_CurLevel->AddObject(pCamObj, 0);



	// GameObject 생성
	CGameObject* pObj = new CGameObject;

	pObj->SetName(L"Player");

	// 오브젝트에 컴포넌트, 렌더컴포넌트, 스크립트 추가
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CPlayerScript);

	// Transform 정보 설정 (1.f->1px)
	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

	// 매쉬, 쉐이더 설정
	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"Std2DShader"));


	CGameObject* pChildObj = new CGameObject;
	pChildObj->SetName(L"Child");

	pChildObj->AddComponent(new CTransform);
	pChildObj->AddComponent(new CMeshRender);

	pChildObj->Transform()->SetRelativePos(Vec3(200.f, 0.f, 0.f));
	pChildObj->Transform()->SetRelativeScale(Vec3(150.f, 150.f, 1.f));
	pChildObj->Transform()->SetAbsolute(true);

	pChildObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pChildObj->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"Std2DShader"));

	pObj->AddChild(pChildObj);



	// 레벨에 추가
	m_CurLevel->AddObject(pObj, 0, false);
}

void CLevelMgr::tick()
{
	if (nullptr == m_CurLevel)
		return;

	// 이전 프레임에 등록했던 오브젝트 초기화
	m_CurLevel->clear();

	// 이후 새로 레벨에서 등록
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
