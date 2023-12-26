#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"

// �θ� �� ���ǵ� �⺻ �����ڰ� ���� ������, ���ڷ� �ڽ� Ŭ������ �ش��ϴ� ���� Ÿ���� �־��ش�.
CMesh::CMesh()
    : CAsset(ASSET_TYPE::MESH) 
    , m_VtxCount(0)
    , m_IdxCount(0)
    , m_VtxSysMem(nullptr)
    , m_IdxSysMem(nullptr)
    , m_VBDesc{}
    , m_IBDesc{}
{
}

CMesh::~CMesh()
{
    IF_DELETE(m_VtxSysMem);
    IF_DELETE(m_IdxSysMem);
}

int CMesh::Create(void* _Vtx, UINT _VtxCount, void* _Idx, UINT _IdxCount)
{
    // Count ����
    m_VtxCount = _VtxCount;
    m_IdxCount = _IdxCount;

    // ���ؽ� ���� desc �ۼ�
    m_VBDesc = {};

    m_VBDesc.ByteWidth           = sizeof(Vtx) * m_VtxCount; // ������ ũ��(Byte)
    m_VBDesc.StructureByteStride = sizeof(Vtx); // ���� ����ü�� ũ��(����ü�� ���� ���� ���� ��� 0)
    m_VBDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // ���ε�: D3D11_BIND_VERTEX_BUFFER (���� ����)
    m_VBDesc.CPUAccessFlags      = 0; // CPU�� GPU�޸� ���� ��� ����: 0(����) (D3D11_CPU_ACCESS_FLAGS) 
    m_VBDesc.Usage               = D3D11_USAGE_DEFAULT; // ���� ��� ���: �⺻

    // �ε��� ���� desc �ۼ�
    m_IBDesc = {};

    m_IBDesc.ByteWidth           = sizeof(UINT) * _IdxCount;
    m_IBDesc.StructureByteStride = sizeof(UINT);
    m_IBDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER; // ���ε�: D3D11_BIND_INDEX_BUFFER (�ε��� ����)
    m_IBDesc.CPUAccessFlags      = 0;
    m_IBDesc.Usage               = D3D11_USAGE_DEFAULT;


    // ���� ���ҽ� �ʱ�ȭ - GPU �޸� ����ϴ� �ֵ��� ������. ����, �ؽ��� ���� �����.
    D3D11_SUBRESOURCE_DATA tSubResourceData{};

    // ���ؽ� ���� ����
    tSubResourceData.pSysMem = _Vtx; // �̰� ���۶�� �˷��ֱ�.
    IF_FAILED(DEVICE->CreateBuffer(&m_VBDesc, &tSubResourceData, m_VB.GetAddressOf())
        , L"Vertex Buffer ����");

    // �ε��� ���� ����
    tSubResourceData.pSysMem = _Idx;
    IF_FAILED(DEVICE->CreateBuffer(&m_IBDesc, &tSubResourceData, m_IB.GetAddressOf())
        , L"Index Buffer ����");

    // ���� ���� ������ �ε��� ������ ���� �Ҵ� �� ������� ����
    m_VtxSysMem = new Vtx[m_VtxCount];
    m_IdxSysMem = new UINT[m_IdxCount];

    memcpy(m_VtxSysMem, _Vtx, sizeof(Vtx) * m_VtxCount);
    memcpy(m_IdxSysMem, _Idx, sizeof(UINT) * m_IdxCount);

    return S_OK;
}

// IA���� ����� �� �ֵ��� ���ؽ� ���ۿ� �ε��� ���� ����
void CMesh::UpdateData()
{
    UINT iStride = sizeof(Vtx);
    UINT iOffset = 0;

    CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &iStride, &iOffset);
    CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::render()
{
    // ���� ������Ʈ(Set)
    UpdateData();
    
    // ���ڷ� �ε��� ���� ũ��, �׸��� ������ �ε���, �׸��� �����ϴ� ������ �ε���
    CONTEXT->DrawIndexed(m_IdxCount, 0, 0);
}

