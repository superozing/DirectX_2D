#include "pch.h"
#include "CMeshRender.h"

// renderComponent�� �ʿ��� �⺻���� 2����
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
	// ���̴� ����
	if (nullptr != GetShader())
	{
		GetShader()->UpdateData();
	}
	// ��� ���� ����
	GetOwner()->Transform()->UpdateData();

}

void CMeshRender::render()
{
	// �� �� �ϳ��� ���� ��쿡 ���� ����ó��
	if (nullptr == GetMesh() || nullptr == GetShader())
		return;

	// ���̴�, ��� ���� ���ε�
	UpdateData();

	// ���� ����, �ε��� ���� ���ε� �� render
	GetMesh()->render();
}
