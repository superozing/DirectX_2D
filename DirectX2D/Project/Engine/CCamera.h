#pragma once
#include "CComponent.h"

// 투영 방식 (직교, 원근)
enum class PROJ_TYPE
{
    ORTHOGRAPHIC, // 직교 투영 -> 깊이가 커도 일정한 1대1 비율
    PERSPECTIVE,  // 원근 투영 -> 깊이와 시야 각에 따라 물체의 크기 변화
};

class CCamera :
    public CComponent
{
private:
    PROJ_TYPE   m_ProjType;     // 투영 방식

    // 원근투영(Perspective)
    float       m_FOV;          // 시야 각 (원근 투영)

    // 직교투영(Orthographic)
    float       m_Width;        // 직교투영 가로 길이
    float       m_Scale;        // 직교투영 배율

    // Both
    float       m_AspectRatio;  // 종횡비, 투영 가로세로 비율
    float       m_Far;          // 투영 최대 거리

    // 변환 행렬
    Matrix      m_matView;
    Matrix      m_matProj;

public: //Get, Set 함수
    PROJ_TYPE   GetProjType() const             { return m_ProjType; }
    void        SetProjType(PROJ_TYPE _Type)    { m_ProjType = _Type; }

    float       GetScale() const                { return m_Scale; }
    void        SetScale(float _Scale)          { m_Scale = _Scale; }

    float       GetFOV() const                  { return m_FOV; }
    void        SetFOV(float _FOV)              { m_FOV = _FOV; }

public:
    virtual void finaltick() override;


public:
    CCamera();
    ~CCamera();
};

