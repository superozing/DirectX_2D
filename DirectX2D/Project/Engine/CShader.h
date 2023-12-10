#pragma once
#include "CAsset.h"


class CShader :
    public CAsset
{
protected:
    ComPtr<ID3DBlob>    m_ErrBlob; // 오류가 날 경우 에러 블롭을 채움.


public:
    virtual void UpdateData() = 0;


public:
    CShader(ASSET_TYPE _Type);
    ~CShader();
};

