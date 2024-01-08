#pragma once
#include "singleton.h"

#include "CPathMgr.h"
#include "CAsset.h"

#include "CGraphicsShader.h"
#include "CMesh.h"

class CAssetMgr :
    public CSingleton<CAssetMgr>
{
    SINGLE(CAssetMgr); // 싱글턴 화
private:
    map<wstring, CAsset*> m_mapAsset[(UINT)ASSET_TYPE::END];    // 기존 에셋 매니저와 비슷하게 map으로 관리.

public:
    void init();

public:
    template <typename T>
    void AddAsset(const wstring& _strKey, T* _Asset);

    template<typename T>
    T* FindAsset(const wstring& _strKey);

    template<typename T>
    T* Load(const wstring& _strKey, const wstring& _strRelativePath);

};

// AddAsset과 FindAsset 함수에서 typeid 비교를 통해서 에셋의 타입을 반환해줌.
template<typename T>
ASSET_TYPE GetAssetType()
{
    // T의 typeid를 받아옴.
    const type_info& info = typeid(T);

    ASSET_TYPE type = ASSET_TYPE::END;

    // 만약 T의 타입ID와 CMesh의 타입ID가 같다면 T는 CMesh의 객체이다
    if (&info == &typeid(CMesh))
        type = ASSET_TYPE::MESH;
    else if (&info == &typeid(CGraphicsShader))
        type = ASSET_TYPE::GRAPHICS_SHADER;

    return type;
}

template<typename T>
inline void CAssetMgr::AddAsset(const wstring& _strKey, T* _Asset)
{
    ASSET_TYPE Type = GetAssetType<T>();
    auto iter = m_mapAsset[(UINT)Type].find(_strKey);
    assert(iter == m_mapAsset[(UINT)Type].end());// 만약 이미 이전에 데이터를 넣은 적이 있다면 assert

    // 없다면 insert
    m_mapAsset[(UINT)Type].insert({ _strKey, _Asset });
}

template<typename T>
inline T* CAssetMgr::FindAsset(const wstring& _strKey)
{
    ASSET_TYPE Type = GetAssetType<T>();
    map<wstring, CAsset*>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);

    if (iter == m_mapAsset[(UINT)Type].end()) // 만약 해당 데이터가 없을 경우 nullptr 반환
        return nullptr;
    // 해당 키 값 데이터의 second(에셋)을 반환
    return (T*)iter->second;
}

template<typename T>
inline T* CAssetMgr::Load(const wstring& _strKey, const wstring& _strRelativePath)
{
    CAsset* pAsset = FindAsset<T>(_strKey);

    // 로딩할 때 사용할 키로 이미 다른 에셋이 있다면 해당 키 값의 에셋 반환
    if (nullptr != pAsset)
        return (T*)pAsset;

    wstring strFilePath = CPathMgr::GetContentPath() + _strRelativePath;

    pAsset = new T;

    if (FAILED(pAsset->Load(strFilePath)))
    {
        MessageBox(nullptr, L"에셋 로딩 실패", L"에셋 로딩 실패", MB_OK);
        delete pAsset;
        return nullptr;
    }

    // 에셋 로딩 성공 시 map에 추가
    pAsset->SetKey(_strKey);
    pAsset->SetRelativePath(_strRelativePath);

    AddAsset(_strKey, pAsset);

    return (T*)pAsset;
}