#include "pch.h"
#include "CGraphicsShader.h"

#include "CDevice.h"
#include "CPathMgr.h"


CGraphicsShader::CGraphicsShader()
	: CShader(ASSET_TYPE::GRAPHICS_SHADER)
	, m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) // 도형 렌더링 방식: 세 개의 정점을 삼각형으로 연결함
{
}

CGraphicsShader::~CGraphicsShader()
{
}

int CGraphicsShader::CreateVertexShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	// Vertex Shader
	// HLSL VS_Std2D 함수 컴파일
	wstring conPath = CPathMgr::GetContentPath();
	wstring filePath = conPath + _strRelativePath; // 컨텐츠 경로 + 상대 경로

	// VSBlob 채우기
	if (FAILED(D3DCompileFromFile(filePath.c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "vs_5_0", D3DCOMPILE_DEBUG, 0
		, m_VSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (nullptr != m_ErrBlob) // 오류가 날 경우 에러 블롭에 오류 정보를 채워줌.
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			string captionName = "[CGraphicsShader.cpp]" + _strFuncName + "컴파일 실패";
			MessageBoxA(nullptr, pErrMsg, captionName.c_str(), MB_OK);
		}

		return E_FAIL;
	}

	// VertexShader 생성
	DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer()
		, m_VSBlob->GetBufferSize(), nullptr
		, m_VS.GetAddressOf());

	/* 현재 정점 구조
	struct Vtx
	{
		Vec3 vPos;		 // 정점의 좌표
		Vec4 vColor;	 // 정점의 색상 정보
		Vec2 vUV;		 // UV 좌표계 || Texture Coodinate
	};
	*/

	// 정점 구조 정보(Element Desc) 생성
	D3D11_INPUT_ELEMENT_DESC arrElement[3] = {};

	// Vec3 vPos - 정점의 좌표
	arrElement[0].InputSlot = 0;				// 입력 데이터가 어느 입력 슬롯에 위치하는지. 여러 입력 슬롯이 들어갈 경우에만 사용.
	arrElement[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // VERTEX인지 INSTANCE인지 설정
	arrElement[0].SemanticName = "POSITION";	// 시맨틱: HLSL 쪽에 정의된 정점 구조와 연결을 위해 사용.
	arrElement[0].SemanticIndex = 0;			// 시맨틱 인덱스: 같은 이름의 시맨틱을 여러 개 사용할 경우, 시맨틱 인덱스를 다르게 해서 사용 가능
	arrElement[0].InstanceDataStepRate = 0;		// 몇 번의 인스턴스 마다 새 데이터가 나오는지. 인스턴싱 사용시에만 사용
	arrElement[0].AlignedByteOffset = 0;		// 입력 데이터가 정점 데이터에서 몇 바이트 떨어져 시작하는지
	arrElement[0].Format = DXGI_FORMAT_R32G32B32_FLOAT; // 포멧: 정점 위치 정보가 3D 벡터일 경우 보통 이 포멧을 사용한다.

	// Vec4 vColor - 정점의 색상 정보
	arrElement[1].InputSlot = 0;
	arrElement[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[1].SemanticName = "COLOR";
	arrElement[1].SemanticIndex = 0;
	arrElement[1].InstanceDataStepRate = 0;
	arrElement[1].AlignedByteOffset = 12;
	arrElement[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	// Vec2 vUV - UV 좌표계, Texture Coodinate
	arrElement[2].InputSlot = 0;
	arrElement[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[2].SemanticName = "TEXCOORD";
	arrElement[2].SemanticIndex = 0;
	arrElement[2].InstanceDataStepRate = 0;
	arrElement[2].AlignedByteOffset = 28;
	arrElement[2].Format = DXGI_FORMAT_R32G32_FLOAT;

	// Layout 생성
	DEVICE->CreateInputLayout(arrElement, 3
				, m_VSBlob->GetBufferPointer()
				, m_VSBlob->GetBufferSize()
				, m_Layout.GetAddressOf());

	return S_OK;
}

int CGraphicsShader::CreateHullShader()
{
	return S_OK;
}

int CGraphicsShader::CreateDomainShader()
{
	return S_OK;
}

int CGraphicsShader::CreateGeometryShader()
{
	return S_OK;
}

int CGraphicsShader::CreatePixelShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	// Pixel Shader
	// HLSL PS_Std2D 함수 컴파일
	wstring conPath = CPathMgr::GetContentPath();
	wstring filePath = conPath + _strRelativePath; // 컨텐츠 경로 + 상대 경로

	if (FAILED(D3DCompileFromFile(filePath.c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "ps_5_0", D3DCOMPILE_DEBUG, 0
		, m_PSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (nullptr != m_ErrBlob) // 오류가 날 경우 에러 블롭에 오류 정보를 채워줌.
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			string captionName = "[CGraphicsShader.cpp]" + _strFuncName + "컴파일 실패";
			MessageBoxA(nullptr, pErrMsg, captionName.c_str(), MB_OK);
		}

		return E_FAIL;
	}
	// PixelShader 생성
	DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer()
		, m_PSBlob->GetBufferSize(), nullptr
		, m_PS.GetAddressOf());

	return S_OK;
}


// 그래픽스 파이프라인에 레이아웃, 도형 렌더링 방식, 쉐이더  세팅
// 세팅하지 않고 기본 값을 사용하더라도 nullptr로 밀어주어야 함.
void CGraphicsShader::UpdateData()
{
	CONTEXT->IASetInputLayout(m_Layout.Get());
	CONTEXT->IASetPrimitiveTopology(m_Topology);

	CONTEXT->VSSetShader(m_VS.Get(), nullptr, 0); // 뒤의 인자 2개는 instance 사용 시 입력
	CONTEXT->HSSetShader(m_HS.Get(), nullptr, 0);
	CONTEXT->DSSetShader(m_DS.Get(), nullptr, 0);
	CONTEXT->GSSetShader(m_GS.Get(), nullptr, 0);
	CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0);
}
