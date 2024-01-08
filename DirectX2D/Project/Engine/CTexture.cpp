#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"

CTexture::CTexture()
	: CAsset(ASSET_TYPE::TEXTURE)
	, m_Desc{}
{
}

CTexture::~CTexture()
{
}

int CTexture::Load(const wstring& _strFilePath)
{
	// 텍스쳐 확장자
	wchar_t szExt[20] = {};
	_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 20);

	HRESULT hr = S_OK;

	if (!wcscmp(szExt, L".dds") || !wcscmp(szExt, L".DDS"))
		hr = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
	else if (!wcscmp(szExt, L".tga") || !wcscmp(szExt, L".TGA"))
		hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
	else
		hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image); // png, bmp, jpg, jpeg

	IF_FAILED(hr, L"[CTexture.cpp]Load() - 텍스쳐 로딩 오류")

	// 1. ID3D11Texture2D 객체 생성
	// 2. ScratchImage 에 있는 데이터를 ID3D11Texture2D 객체 로 전달
	// 3. ID3D11Texture2D 를 이용한 ShaderResourceView 만들기
	CreateShaderResourceView(DEVICE
		, m_Image.GetImages()
		, m_Image.GetImageCount()
		, m_Image.GetMetadata()
		, m_SRV.GetAddressOf());

	// 만들어진 ShaderResourceView 를 이용해서 생성된 ID3D11Texture2D 객체를 알아낸다.
	m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());

	// ID3D11Texture2D 객체를 통해서 생성될때 사용된 옵션(DESC) 정보를 가져온다.
	m_Tex2D->GetDesc(&m_Desc);

	return S_OK;
}

void CTexture::UpdateData(int _RegisterNum)
{
	// 모든 쉐이더 갈아 끼움
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}