#include "pch.h"
#include "CTexture.h"

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

	return S_OK;
}