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
    Vec3    m_arrLocalDir[(UINT)DIR_TYPE::END]; // Right, Up, Front. �� ���� ������ ������ �ݴ� ������ ����Ű�� �ȴ�.
    Vec3    m_arrWorldDir[(UINT)DIR_TYPE::END]; // Right, Up, Front. �� ���� ������ ������ �ݴ� ������ ����Ű�� �ȴ�.

    Matrix  m_matWorld;  // ���� ���

    bool m_bAbsolute; // �θ� ������Ʈ�κ��� 

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

    Vec3 GetLocalDir(DIR_TYPE _type) const { return m_arrLocalDir[(UINT)_type]; }
    Vec3 GetWorldDir(DIR_TYPE _type) const { return m_arrWorldDir[(UINT)_type]; }

    void SetAbsolute(bool _bAbsolute) { m_bAbsolute = _bAbsolute; }

public:
    CTransform();
    ~CTransform();

};

