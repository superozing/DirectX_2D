#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"

// 부모 쪽 정의된 기본 생성자가 없기 때문에, 인자로 자신 클래스가 해당하는 에셋 타입을 넣어준다.
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
    // Count 저장
    m_VtxCount = _VtxCount;
    m_IdxCount = _IdxCount;

    // 버텍스 버퍼 desc 작성
    m_VBDesc = {};

    m_VBDesc.ByteWidth           = sizeof(Vtx) * m_VtxCount; // 버퍼의 크기(Byte)
    m_VBDesc.StructureByteStride = sizeof(Vtx); // 정점 구조체의 크기(구조체가 정의 되지 않을 경우 0)
    m_VBDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // 바인딩: D3D11_BIND_VERTEX_BUFFER (정점 버퍼)
    m_VBDesc.CPUAccessFlags      = 0; // CPU의 GPU메모리 접근 허용 여부: 0(금지) (D3D11_CPU_ACCESS_FLAGS) 
    m_VBDesc.Usage               = D3D11_USAGE_DEFAULT; // 버퍼 사용 방법: 기본

    // 인덱스 버퍼 desc 작성
    m_IBDesc = {};

    m_IBDesc.ByteWidth           = sizeof(UINT) * _IdxCount;
    m_IBDesc.StructureByteStride = sizeof(UINT);
    m_IBDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER; // 바인딩: D3D11_BIND_INDEX_BUFFER (인덱스 버퍼)
    m_IBDesc.CPUAccessFlags      = 0;
    m_IBDesc.Usage               = D3D11_USAGE_DEFAULT;


    // 서브 리소스 초기화 - GPU 메모리 사용하는 애들의 데이터. 버퍼, 텍스쳐 등이 사용함.
    D3D11_SUBRESOURCE_DATA tSubResourceData{};

    // 버텍스 버퍼 생성
    tSubResourceData.pSysMem = _Vtx; // 이게 버퍼라고 알려주기.
    IF_FAILED(DEVICE->CreateBuffer(&m_VBDesc, &tSubResourceData, m_VB.GetAddressOf())
        , L"Vertex Buffer 생성");

    // 인덱스 버퍼 생성
    tSubResourceData.pSysMem = _Idx;
    IF_FAILED(DEVICE->CreateBuffer(&m_IBDesc, &tSubResourceData, m_IB.GetAddressOf())
        , L"Index Buffer 생성");

    // 원본 정점 정보와 인덱스 정보를 동적 할당 후 저장시켜 관리
    m_VtxSysMem = new Vtx[m_VtxCount];
    m_IdxSysMem = new UINT[m_IdxCount];

    memcpy(m_VtxSysMem, _Vtx, sizeof(Vtx) * m_VtxCount);
    memcpy(m_IdxSysMem, _Idx, sizeof(UINT) * m_IdxCount);

    return S_OK;
}

// IA에서 사용할 수 있도록 버텍스 버퍼와 인덱스 버퍼 세팅
void CMesh::UpdateData()
{
    UINT iStride = sizeof(Vtx);
    UINT iOffset = 0;

    CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &iStride, &iOffset);
    CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::render()
{
    // 버퍼 업데이트(Set)
    UpdateData();
    
    // 인자로 인덱스 버퍼 크기, 그리기 시작할 인덱스, 그리기 시작하는 정점의 인덱스
    CONTEXT->DrawIndexed(m_IdxCount, 0, 0);
}

