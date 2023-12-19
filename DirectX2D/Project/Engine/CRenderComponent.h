#pragma once
#include "CComponent.h"

class CMesh;
class CGraphicsShader;

class CRenderComponent :
    public CComponent
{
private: 
    // ��ü�� �׸��� ���� �ʿ��� �⺻�� 2����
    CMesh*           m_Mesh;
    CGraphicsShader* m_Shader;

public: // Get, Set �Լ�

    void SetMesh(CMesh* _Mesh) { m_Mesh = _Mesh; }
    void SetShader(CGraphicsShader* _Shader) { m_Shader = _Shader; }

    CMesh* GetMesh() { return m_Mesh; }
    CGraphicsShader* GetShader() { return m_Shader; }


public:
    virtual void finaltick() {}; // finaltick()�� ������� �ʴ� render������Ʈ�� �ֱ� ������ ���������Լ��� �������� ����.
    virtual void UpdateData() = 0;
    virtual void render() = 0;

public:
    CRenderComponent(COMPONENT_TYPE _type);
    ~CRenderComponent();
    
};

