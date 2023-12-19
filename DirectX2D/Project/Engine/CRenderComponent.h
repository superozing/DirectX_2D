#pragma once
#include "CComponent.h"

class CMesh;
class CGraphicsShader;

class CRenderComponent :
    public CComponent
{
private: 
    // 물체를 그리기 위해 필요한 기본적 2가지
    CMesh*           m_Mesh;
    CGraphicsShader* m_Shader;

public: // Get, Set 함수

    void SetMesh(CMesh* _Mesh) { m_Mesh = _Mesh; }
    void SetShader(CGraphicsShader* _Shader) { m_Shader = _Shader; }

    CMesh* GetMesh() { return m_Mesh; }
    CGraphicsShader* GetShader() { return m_Shader; }


public:
    virtual void finaltick() {}; // finaltick()을 사용하지 않는 render컴포넌트도 있기 때문에 순수가상함수로 정의하지 않음.
    virtual void UpdateData() = 0;
    virtual void render() = 0;

public:
    CRenderComponent(COMPONENT_TYPE _type);
    ~CRenderComponent();
    
};

