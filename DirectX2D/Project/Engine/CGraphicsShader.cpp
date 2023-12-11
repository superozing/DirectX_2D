#include "pch.h"
#include "CGraphicsShader.h"

#include "CDevice.h"
#include "CPathMgr.h"


CGraphicsShader::CGraphicsShader()
	: CShader(ASSET_TYPE::GRAPHICS_SHADER)
	, m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) // ���� ������ ���: �� ���� ������ �ﰢ������ ������
{
}

CGraphicsShader::~CGraphicsShader()
{
}

int CGraphicsShader::CreateVertexShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	// Vertex Shader
	// HLSL VS_Std2D �Լ� ������
	wstring conPath = CPathMgr::GetContentPath();
	wstring filePath = conPath + _strRelativePath; // ������ ��� + ��� ���

	// VSBlob ä���
	if (FAILED(D3DCompileFromFile(filePath.c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "vs_5_0", D3DCOMPILE_DEBUG, 0
		, m_VSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (nullptr != m_ErrBlob) // ������ �� ��� ���� ��ӿ� ���� ������ ä����.
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			string captionName = "[CGraphicsShader.cpp]" + _strFuncName + "������ ����";
			MessageBoxA(nullptr, pErrMsg, captionName.c_str(), MB_OK);
		}

		return E_FAIL;
	}

	// VertexShader ����
	DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer()
		, m_VSBlob->GetBufferSize(), nullptr
		, m_VS.GetAddressOf());

	/* ���� ���� ����
	struct Vtx
	{
		Vec3 vPos;		 // ������ ��ǥ
		Vec4 vColor;	 // ������ ���� ����
		Vec2 vUV;		 // UV ��ǥ�� || Texture Coodinate
	};
	*/

	// ���� ���� ����(Element Desc) ����
	D3D11_INPUT_ELEMENT_DESC arrElement[3] = {};

	// Vec3 vPos - ������ ��ǥ
	arrElement[0].InputSlot = 0;				// �Է� �����Ͱ� ��� �Է� ���Կ� ��ġ�ϴ���. ���� �Է� ������ �� ��쿡�� ���.
	arrElement[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // VERTEX���� INSTANCE���� ����
	arrElement[0].SemanticName = "POSITION";	// �ø�ƽ: HLSL �ʿ� ���ǵ� ���� ������ ������ ���� ���.
	arrElement[0].SemanticIndex = 0;			// �ø�ƽ �ε���: ���� �̸��� �ø�ƽ�� ���� �� ����� ���, �ø�ƽ �ε����� �ٸ��� �ؼ� ��� ����
	arrElement[0].InstanceDataStepRate = 0;		// �� ���� �ν��Ͻ� ���� �� �����Ͱ� ��������. �ν��Ͻ� ���ÿ��� ���
	arrElement[0].AlignedByteOffset = 0;		// �Է� �����Ͱ� ���� �����Ϳ��� �� ����Ʈ ������ �����ϴ���
	arrElement[0].Format = DXGI_FORMAT_R32G32B32_FLOAT; // ����: ���� ��ġ ������ 3D ������ ��� ���� �� ������ ����Ѵ�.

	// Vec4 vColor - ������ ���� ����
	arrElement[1].InputSlot = 0;
	arrElement[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[1].SemanticName = "COLOR";
	arrElement[1].SemanticIndex = 0;
	arrElement[1].InstanceDataStepRate = 0;
	arrElement[1].AlignedByteOffset = 12;
	arrElement[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	// Vec2 vUV - UV ��ǥ��, Texture Coodinate
	arrElement[2].InputSlot = 0;
	arrElement[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[2].SemanticName = "TEXCOORD";
	arrElement[2].SemanticIndex = 0;
	arrElement[2].InstanceDataStepRate = 0;
	arrElement[2].AlignedByteOffset = 28;
	arrElement[2].Format = DXGI_FORMAT_R32G32_FLOAT;

	// Layout ����
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
	// HLSL PS_Std2D �Լ� ������
	wstring conPath = CPathMgr::GetContentPath();
	wstring filePath = conPath + _strRelativePath; // ������ ��� + ��� ���

	if (FAILED(D3DCompileFromFile(filePath.c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "ps_5_0", D3DCOMPILE_DEBUG, 0
		, m_PSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (nullptr != m_ErrBlob) // ������ �� ��� ���� ��ӿ� ���� ������ ä����.
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			string captionName = "[CGraphicsShader.cpp]" + _strFuncName + "������ ����";
			MessageBoxA(nullptr, pErrMsg, captionName.c_str(), MB_OK);
		}

		return E_FAIL;
	}
	// PixelShader ����
	DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer()
		, m_PSBlob->GetBufferSize(), nullptr
		, m_PS.GetAddressOf());

	return S_OK;
}


// �׷��Ƚ� ���������ο� ���̾ƿ�, ���� ������ ���, ���̴�  ����
// �������� �ʰ� �⺻ ���� ����ϴ��� nullptr�� �о��־�� ��.
void CGraphicsShader::UpdateData()
{
	CONTEXT->IASetInputLayout(m_Layout.Get());
	CONTEXT->IASetPrimitiveTopology(m_Topology);

	CONTEXT->VSSetShader(m_VS.Get(), nullptr, 0); // ���� ���� 2���� instance ��� �� �Է�
	CONTEXT->HSSetShader(m_HS.Get(), nullptr, 0);
	CONTEXT->DSSetShader(m_DS.Get(), nullptr, 0);
	CONTEXT->GSSetShader(m_GS.Get(), nullptr, 0);
	CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0);
}
