#pragma once
#include "CComponent.h"

class CTransform :
    public CComponent
{
private:
    Vec3    m_vRelativePos;        // 위치 정보
    Vec3    m_vRelativeScale;      // 크기 정보(배율)
    Vec3    m_vRealtiveRotation;   // 회전 정보

    // 방향 정보
    Vec3    m_arrDir[(UINT)DIR_TYPE::END]; // Right, Up, Front. 이 값을 음수로 놓으면 반대 방향을 가리키게 된다.

    Matrix  m_matWorld;  // 상태 행렬

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;

public:// Get, Set 함수
    void SetRelativePos(Vec3 _Pos) { m_vRelativePos = _Pos; }
    void SetRelativeScale(Vec3 _Scale) { m_vRelativeScale = _Scale; }
    void SetRelativeRotation(Vec3 _Rotation) { m_vRealtiveRotation = _Rotation; }

    Vec3 GetRelativePos() const { return m_vRelativePos; }
    Vec3 GetRelativeScale() const { return m_vRelativeScale; }
    Vec3 GetRelativeRotation() const { return m_vRealtiveRotation; }

    const Matrix& GetWorldMat() { return m_matWorld; }

    Vec3 GetDir(DIR_TYPE _type) const { return m_arrDir[(UINT)_type]; }

public:
    CTransform();
    ~CTransform();

};

