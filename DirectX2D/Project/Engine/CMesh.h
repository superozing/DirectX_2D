#pragma once
#include "CAsset.h"

// 정점 버퍼 + 인덱스 버퍼 관리
class CMesh :
    public CAsset
{
private:
    // buffer
    ComPtr<ID3D11Buffer>    m_VB; // Vertex Buffer
    ComPtr<ID3D11Buffer>    m_IB; // Index Buffer
    
    // description
    D3D11_BUFFER_DESC       m_VBDesc;
    D3D11_BUFFER_DESC       m_IBDesc;

    // count
    UINT                    m_VtxCount;
    UINT                    m_IdxCount;

    // 혹시 몰라 저장하는 시스템 메모리 공간
    void*                   m_VtxSysMem;
    void*                   m_IdxSysMem;


private:
    void UpdateData();

public:
    // 정점 배열 또는 벡터, 정점 개수, 인덱스 배열 또는 벡터, 인덱스 개수
    int Create(void* _Vtx, UINT _VtxCount, void* _Idx, UINT _IdxCount);

    void render();

public:
    CMesh();
    ~CMesh();
};

