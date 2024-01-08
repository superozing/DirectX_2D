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
	// �ؽ��� Ȯ����
	wchar_t szExt[20] = {};
	_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 20);

	HRESULT hr = S_OK;

	if (!wcscmp(szExt, L".dds") || !wcscmp(szExt, L".DDS"))
		hr = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
	else if (!wcscmp(szExt, L".tga") || !wcscmp(szExt, L".TGA"))
		hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
	else
		hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image); // png, bmp, jpg, jpeg

	IF_FAILED(hr, L"[CTexture.cpp]Load() - �ؽ��� �ε� ����")

	// 1. ID3D11Texture2D ��ü ����
	// 2. ScratchImage �� �ִ� �����͸� ID3D11Texture2D ��ü �� ����
	// 3. ID3D11Texture2D �� �̿��� ShaderResourceView �����
	CreateShaderResourceView(DEVICE
		, m_Image.GetImages()
		, m_Image.GetImageCount()
		, m_Image.GetMetadata()
		, m_SRV.GetAddressOf());

	// ������� ShaderResourceView �� �̿��ؼ� ������ ID3D11Texture2D ��ü�� �˾Ƴ���.
	m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());

	// ID3D11Texture2D ��ü�� ���ؼ� �����ɶ� ���� �ɼ�(DESC) ������ �����´�.
	m_Tex2D->GetDesc(&m_Desc);

	return S_OK;
}

void CTexture::UpdateData(int _RegisterNum)
{
	// ��� ���̴� ���� ����
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}