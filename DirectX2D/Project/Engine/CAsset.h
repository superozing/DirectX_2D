#pragma once
#include "CEntity.h"

class CAsset :
    public CEntity
{
private:
    wstring             m_Key; // 키 값
    wstring             m_RelativePath; // 상대 경로
    int                 m_RefCount; // 참조 개수 (스마트 포인터)
    const ASSET_TYPE    m_Type;


public: // Get, Set
    const wstring& GetKey()          { return m_Key; }
    const wstring& GetRelativePath() { return m_RelativePath; }

private:
    void SetKey(const wstring& _key) { m_Key = _key; }
    void SetRelativePath(const wstring& _RelativePath) { m_RelativePath = m_RelativePath; }

    void AddRef() { ++m_RefCount; }
    void SubRef() { --m_RefCount; }

    int GetRefCount() const { return m_RefCount; }

    ASSET_TYPE GetAssetType() const { return m_Type; }

public:
    CAsset(ASSET_TYPE _Type);
    ~CAsset();
};

