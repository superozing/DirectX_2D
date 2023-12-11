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

// 상수 데이터를 전달하는 버퍼
ComPtr<ID3D11Buffer>	g_CB = nullptr;

// 화면 클리어 색상
float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };


int TestInit()
{
	// 전역변수에 삼각형 위치 설정
//   0,3(Red)-- 1(Blue)	     
//    |   \   |	     
//   3(G)---- 2,4(Magenta)  
// 정점 4개, 인덱스 6개
	
	// 정점 버퍼
	Vtx arrVtx[4]{};
	
	arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);		// ndc 좌표계
	arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);	// red
	arrVtx[0].vUV = Vec2(0.f, 0.f);					// 아직 사용하지 않음

	arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vColor = Vec4(0.f, 0.f, 1.f, 1.f);	// blue
	arrVtx[1].vUV = Vec2(0.f, 0.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vColor = Vec4(1.f, 0.f, 1.f, 1.f);	// green
	arrVtx[2].vUV = Vec2(0.f, 0.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vColor = Vec4(0.f, 1.f, 0.f, 1.f);	// magenta
	arrVtx[3].vUV = Vec2(0.f, 0.f);

	// 인덱스 버퍼
	UINT arrIdx[6]{};

	arrIdx[0] = 0;
	arrIdx[1] = 1;
	arrIdx[2] = 2;
	arrIdx[3] = 0;
	arrIdx[4] = 2;
	arrIdx[5] = 3;

	g_RectMesh = new CMesh;
	g_RectMesh->Create(arrVtx, 4, arrIdx, 6);


	// CircleMesh 만들기
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;

	// 중심 점
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



	// 상수 버퍼(Constant Buffer) 생성
	D3D11_BUFFER_DESC BufferDesc{};

	BufferDesc.ByteWidth = sizeof(tTransform);
	BufferDesc.StructureByteStride = sizeof(tTransform);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// 버퍼에 데이터 쓰기 가능(트랜스폼 조절을 위해)
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;

	// 상수 버퍼(transform) 생성
	IF_FAILED(DEVICE->CreateBuffer(&BufferDesc, nullptr, g_CB.GetAddressOf()), L"상수 버퍼 (Transform)생성");

	// CGraphicsShader 생성
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

	// GPU 쪽 메모리를 Map해서 새로 입력된 트랜스폼 데이터로 교체
	CONTEXT->Map(g_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, &g_Transform, sizeof(tTransform));
	CONTEXT->Unmap(g_CB.Get(), 0);
}

void Render()
{
	INST(CDevice)->ClearView(ClearColor);
	
	// 상수 버퍼 세팅
	CONTEXT->VSSetConstantBuffers(0, 1, g_CB.GetAddressOf());

	// 쉐이더 업데이트 후 세팅
	g_Shader->UpdateData();

	// 메시 선택 및 렌더
	g_RectMesh->render();

	// 메인 화면으로 스왑 체인
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
