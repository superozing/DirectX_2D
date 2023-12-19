#include "pch.h"
#include "CMeshRender.h"

// renderComponent에 필요한 기본적인 2가지
#include "CMesh.h"
#include "CGraphicsShader.h"

//
#include "CGameObject.h"
#include "CTransform.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::UpdateData()
{
	// 쉐이더 세팅
	if (nullptr != GetShader())
	{
		GetShader()->UpdateData();
	}
	// 상수 버퍼 세팅
	GetOwner()->Transform()->UpdateData();

}

void CMeshRender::render()
{
	// 둘 중 하나라도 없을 경우에 대한 예외처리
	if (nullptr == GetMesh() || nullptr == GetShader())
		return;

	// 쉐이더, 상수 버퍼 바인딩
	UpdateData();

	// 정점 버퍼, 인덱스 버퍼 바인딩 후 render
	GetMesh()->render();
}
