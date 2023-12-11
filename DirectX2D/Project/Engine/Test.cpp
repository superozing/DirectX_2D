#include "pch.h"
#include "Test.h"

#include "CDevice.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

tTransform g_Transform = { Vec4(0.f, 0.f, 0.f, 0.f), Vec4(1.f, 1.f, 1.f, 1.f) };

CMesh* g_RectMesh = nullptr;
CMesh* g_CircleMesh = nullptr;

CGraphicsShader* g_Shader = nullptr;

// ��� �����͸� �����ϴ� ����
ComPtr<ID3D11Buffer>	g_CB = nullptr;

// ȭ�� Ŭ���� ����
float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };


int TestInit()
{
	// ���������� �ﰢ�� ��ġ ����
//   0,3(Red)-- 1(Blue)	     
//    |   \   |	     
//   3(G)---- 2,4(Magenta)  
// ���� 4��, �ε��� 6��
	
	// ���� ����
	Vtx arrVtx[4]{};
	
	arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);		// ndc ��ǥ��
	arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);	// red
	arrVtx[0].vUV = Vec2(0.f, 0.f);					// ���� ������� ����

	arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vColor = Vec4(0.f, 0.f, 1.f, 1.f);	// blue
	arrVtx[1].vUV = Vec2(0.f, 0.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vColor = Vec4(1.f, 0.f, 1.f, 1.f);	// green
	arrVtx[2].vUV = Vec2(0.f, 0.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vColor = Vec4(0.f, 1.f, 0.f, 1.f);	// magenta
	arrVtx[3].vUV = Vec2(0.f, 0.f);

	// �ε��� ����
	UINT arrIdx[6]{};

	arrIdx[0] = 0;
	arrIdx[1] = 1;
	arrIdx[2] = 2;
	arrIdx[3] = 0;
	arrIdx[4] = 2;
	arrIdx[5] = 3;

	g_RectMesh = new CMesh;
	g_RectMesh->Create(arrVtx, 4, arrIdx, 6);


	// CircleMesh �����
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;

	// �߽� ��
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	UINT iSlice = 40;
	float fTheta = 0.f;
	float fRadius = 0.5f;

	for (int i = 0; i <= iSlice; ++i)
	{
		fTheta = (XM_2PI / iSlice) * i;

		v.vPos = Vec3(fRadius * cosf(fTheta), fRadius * sinf(fTheta), 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV = Vec2(0.f, 0.f);

		vecVtx.push_back(v);
	}

	for (int i = 0; i < iSlice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	g_CircleMesh = new CMesh;
	g_CircleMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());



	// ��� ����(Constant Buffer) ����
	D3D11_BUFFER_DESC BufferDesc{};

	BufferDesc.ByteWidth = sizeof(tTransform);
	BufferDesc.StructureByteStride = sizeof(tTransform);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// ���ۿ� ������ ���� ����(Ʈ������ ������ ����)
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;

	// ��� ����(transform) ����
	IF_FAILED(DEVICE->CreateBuffer(&BufferDesc, nullptr, g_CB.GetAddressOf()), L"��� ���� (Transform)����");

	// CGraphicsShader ����
	g_Shader = new CGraphicsShader;
	g_Shader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	g_Shader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	return S_OK;
}

void Tick()
{
	if (KEY_PRESSED(KEY::LEFT)) g_Transform.vWorldPos.x -= DT;
	if (KEY_PRESSED(KEY::RIGHT))g_Transform.vWorldPos.x += DT;
	if (KEY_PRESSED(KEY::UP))	g_Transform.vWorldPos.y += DT;
	if (KEY_PRESSED(KEY::DOWN))	g_Transform.vWorldPos.y -= DT;
	if (KEY_PRESSED(KEY::NUM1))	g_Transform.vWorldScale += DT * Vec4(1.f, 1.f, 1.f, 1.f);
	if (KEY_PRESSED(KEY::NUM2))	g_Transform.vWorldScale -= DT * Vec4(1.f, 1.f, 1.f, 1.f);

	// SystemMem -> GPUMem
	D3D11_MAPPED_SUBRESOURCE tSub = {};

	// GPU �� �޸𸮸� Map�ؼ� ���� �Էµ� Ʈ������ �����ͷ� ��ü
	CONTEXT->Map(g_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, &g_Transform, sizeof(tTransform));
	CONTEXT->Unmap(g_CB.Get(), 0);
}

void Render()
{
	INST(CDevice)->ClearView(ClearColor);
	
	// ��� ���� ����
	CONTEXT->VSSetConstantBuffers(0, 1, g_CB.GetAddressOf());

	// ���̴� ������Ʈ �� ����
	g_Shader->UpdateData();

	// �޽� ���� �� ����
	g_RectMesh->render();

	// ���� ȭ������ ���� ü��
	INST(CDevice)->Present();
}

void TestProgress()
{
	Tick();
	Render();
}

void TestRelease()
{
	if (nullptr != g_RectMesh)
	{
		delete g_RectMesh;
	}

	if (nullptr != g_CircleMesh)
	{
		delete g_CircleMesh;
	}

	delete g_Shader;
}
