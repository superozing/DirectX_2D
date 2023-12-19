#pragma once
#include "singleton.h"

#include "CAsset.h"

class CMesh;
class CGraphicsShader;


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

template<typename T>
inline void CAssetMgr::AddAsset(const wstring& _strKey, T* _Asset)
{
    // T의 typeid를 받아옴.
    const type_info& info = typeid(T);
    
    ASSET_TYPE type = ASSET_TYPE::END;

    // 만약 T의 타입ID와 CMesh의 타입ID가 같다면 T는 CMesh의 객체이다
    switch (&info)
    {
    case &typeid(CMesh) :
        type = ASSET_TYPE::MESH;
        break;
    case& typeid(CGraphicsShader) :
        type = ASSET_TYPE::GRAPHICS_SHADER;
        break;
    }
    
    // T의 타입이 잘못 들어왔을 경우
    if (m_mapAsset->end() == m_mapAsset[(UINT)type].find(_strKey))
    {
        IF_FAILED(E_FAIL, "[CAssetMgr.h] 잘못된 타입의 에셋을 추가하려고 했습니다.");
        assert(nullptr);
    }
}

template<typename T>
inline T* CAssetMgr::FindAsset(const wstring& _strKey)
{
    return nullptr;
}
