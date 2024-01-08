#pragma once
#include "CComponent.h"

class CCamera :
    public CComponent
{
private:
    // ��������(Perspective)
    float   m_FOV;          // �þ� ��

    // ��������(Orthographic)
    float   m_Width;        // �������� ���� ����
    float   m_Scale;        // �������� ����

    // Both
    float   m_AspectRatio;  // ��Ⱦ��, ���� ���μ��� ����
    float   m_Far;          // ���� �ִ� �Ÿ�

    // ��ȯ ���
    Matrix  m_matView;
    Matrix  m_matProj;

public:
    virtual void finaltick() override;


public:
    CCamera();
    ~CCamera();
};

