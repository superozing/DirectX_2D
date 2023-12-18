#pragma once
#include "CEntity.h"

class CConstBuffer :
    public CEntity
{
private:
    D3D11_BUFFER_DESC       m_Desc; // 상수 버퍼 정보 (기본적인 상수 버퍼의 정보 + 인자로 들어온 크기 정보)
    ComPtr<ID3D11Buffer>    m_CB; // Constant Buffer

    UINT m_ElementSize; // 상수 버퍼 정보에 입력될 추가적인 정보들
    UINT m_ElementCount;

public:
    int Create(UINT _ElementSize, UINT _ElementCount);


    // 원소 개수 인자가 0일 경우 Create 에서 입력한 기본 값 사용.
    // 이 뜻은, 모든 레지스터를 새로운 데이터로 초기화 하겠다는 뜻.
    // 만약 모든 레지스터를 초기화하고 싶지 않고, 데이터 중 일부만 Set 하고 싶다면 인자 개수를 입력하면 된다.
    void SetData(void* _Src, UINT _ElementCount = 0);
    void UpdateData(UINT _RegisterNum); 


public:
    CConstBuffer();
    ~CConstBuffer();

};

