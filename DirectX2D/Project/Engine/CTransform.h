#pragma once
#include "CComponent.h"

class CTransform :
    public CComponent
{
private:
    Vec3    m_vRelativePos;        // ��ġ ����
    Vec3    m_vRelativeScale;      // ũ�� ����(����)
    Vec3    m_vRealtiveRotation;   // ȸ�� ����

    // ���� ����
    Vec3    m_arrDir[(UINT)DIR_TYPE::END]; // Right, Up, Front. �� ���� ������ ������ �ݴ� ������ ����Ű�� �ȴ�.

    Matrix  m_matWorld;  // ���� ���

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;

public:// Get, Set �Լ�
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

