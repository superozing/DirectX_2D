#pragma once
#include "CAsset.h"

// ���� ���� + �ε��� ���� ����
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

    // Ȥ�� ���� �����ϴ� �ý��� �޸� ����
    void*                   m_VtxSysMem;
    void*                   m_IdxSysMem;


private:
    void UpdateData();

public:
    // ���� �迭 �Ǵ� ����, ���� ����, �ε��� �迭 �Ǵ� ����, �ε��� ����
    int Create(void* _Vtx, UINT _VtxCount, void* _Idx, UINT _IdxCount);

    void render();

public:
    CMesh();
    ~CMesh();
};

