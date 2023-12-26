#pragma once
#include "singleton.h"

#include "CAsset.h"
#include "CGraphicsShader.h"
#include "CMesh.h"



class CAssetMgr :
    public CSingleton<CAssetMgr>
{
    SINGLE(CAssetMgr); // �̱��� ȭ
private:
    map<wstring, CAsset*> m_mapAsset[(UINT)ASSET_TYPE::END];    // ���� ���� �Ŵ����� ����ϰ� map���� ����.

public:
    void init();

public:
    template <typename T>
    void AddAsset(const wstring& _strKey, T* _Asset);

    template<typename T>
    T* FindAsset(const wstring& _strKey);
};

// AddAsset�� FindAsset �Լ����� typeid �񱳸� ���ؼ� ������ Ÿ���� ��ȯ����.
template<typename T>
ASSET_TYPE GetAssetType()
{
    // T�� typeid�� �޾ƿ�.
    const type_info& info = typeid(T);

    ASSET_TYPE type = ASSET_TYPE::END;

    // ���� T�� Ÿ��ID�� CMesh�� Ÿ��ID�� ���ٸ� T�� CMesh�� ��ü�̴�
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
    if (iter != m_mapAsset[(UINT)Type].end()) // ���� �̹� ������ �����͸� ���� ���� �ִٸ� assert
        assert(nullptr);
    // ���ٸ� insert
    m_mapAsset[(UINT)Type].insert({ _strKey, _Asset });
}

template<typename T>
inline T* CAssetMgr::FindAsset(const wstring& _strKey)
{
    ASSET_TYPE Type = GetAssetType<T>();
    map<wstring, CAsset*>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);

    if (iter == m_mapAsset[(UINT)Type].end()) // ���� �ش� �����Ͱ� ���� ��� nullptr ��ȯ
        return nullptr;
    // �ش� Ű �� �������� second(����)�� ��ȯ
    return (T*)iter->second;
}
