#pragma once
#include "CEntity.h"

class CConstBuffer :
    public CEntity
{
private:
    D3D11_BUFFER_DESC       m_Desc; // ��� ���� ���� (�⺻���� ��� ������ ���� + ���ڷ� ���� ũ�� ����)
    ComPtr<ID3D11Buffer>    m_CB; // Constant Buffer

    UINT m_ElementSize; // ��� ���� ������ �Էµ� �߰����� ������
    UINT m_ElementCount;

public:
    int Create(UINT _ElementSize, UINT _ElementCount);


    // ���� ���� ���ڰ� 0�� ��� Create ���� �Է��� �⺻ �� ���.
    // �� ����, ��� �������͸� ���ο� �����ͷ� �ʱ�ȭ �ϰڴٴ� ��.
    // ���� ��� �������͸� �ʱ�ȭ�ϰ� ���� �ʰ�, ������ �� �Ϻθ� Set �ϰ� �ʹٸ� ���� ������ �Է��ϸ� �ȴ�.
    void SetData(void* _Src, UINT _ElementCount = 0);
    void UpdateData(UINT _RegisterNum); 


public:
    CConstBuffer();
    ~CConstBuffer();

};

