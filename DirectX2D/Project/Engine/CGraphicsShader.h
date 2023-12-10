#pragma once
#include "CShader.h"

class CGraphicsShader :
    public CShader
{
private:
    ComPtr<ID3DBlob>    m_VSBlob;       // ���ؽ�
    ComPtr<ID3DBlob>    m_HSBlob;       // ��
    ComPtr<ID3DBlob>    m_DSBlob;       // ������
    ComPtr<ID3DBlob>    m_GSBlob;       // ������Ʈ��
    ComPtr<ID3DBlob>    m_PSBlob;       // �ȼ�

    ComPtr<ID3D11VertexShader>      m_VS;
    ComPtr<ID3D11HullShader>        m_HS;
    ComPtr<ID3D11DomainShader>      m_DS;
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>       m_PS;

    ComPtr<ID3D11InputLayout>       m_Layout;   // ���̾ƿ�: ���� ����

    D3D11_PRIMITIVE_TOPOLOGY        m_Topology; // Topology: ���� ������ ���

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

