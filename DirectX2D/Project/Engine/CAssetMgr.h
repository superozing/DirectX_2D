#pragma once
#include "singleton.h"

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
    if (iter != m_mapAsset[(UINT)Type].end()) // 만약 이미 이전에 데이터를 넣은 적이 있다면 assert
        assert(nullptr);
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
