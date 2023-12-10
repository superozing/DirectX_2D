#pragma once
#include "CShader.h"

class CGraphicsShader :
    public CShader
{
private:
    ComPtr<ID3DBlob>    m_VSBlob;       // 버텍스
    ComPtr<ID3DBlob>    m_HSBlob;       // 헐
    ComPtr<ID3DBlob>    m_DSBlob;       // 도메인
    ComPtr<ID3DBlob>    m_GSBlob;       // 지오메트리
    ComPtr<ID3DBlob>    m_PSBlob;       // 픽셀

    ComPtr<ID3D11VertexShader>      m_VS;
    ComPtr<ID3D11HullShader>        m_HS;
    ComPtr<ID3D11DomainShader>      m_DS;
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>       m_PS;

    ComPtr<ID3D11InputLayout>       m_Layout;   // 레이아웃: 정점 정보

    D3D11_PRIMITIVE_TOPOLOGY        m_Topology; // Topology: 도형 렌더링 방식

    // Rasterizer State

    // OutputMerge
    // DepthStencilState
    // BlendState

public:
    int CreateVertexShader(const wstring& _strRelativePath, const string& _strFuncName);
    int CreateHullShader();
    int CreateDomainShader();
    int CreateGeometryShader();
    int CreatePixelShader(const wstring& _strRelativePath, const string& _strFuncName);

    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { m_Topology = _Topology; }

public:
    virtual void UpdateData();


private:
    int CompileStd2D(const wstring& _strRelativePath, const string& _strFuncName);

public:
    CGraphicsShader();
    ~CGraphicsShader();
};

