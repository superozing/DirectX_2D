#pragma once
#include "singleton.h"

#include "CAsset.h"

class CMesh;
class CGraphicsShader;


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

template<typename T>
inline void CAssetMgr::AddAsset(const wstring& _strKey, T* _Asset)
{
    // T�� typeid�� �޾ƿ�.
    const type_info& info = typeid(T);
    
    ASSET_TYPE type = ASSET_TYPE::END;

    // ���� T�� Ÿ��ID�� CMesh�� Ÿ��ID�� ���ٸ� T�� CMesh�� ��ü�̴�
    switch (&info)
    {
    case &typeid(CMesh) :
        type = ASSET_TYPE::MESH;
        break;
    case& typeid(CGraphicsShader) :
        type = ASSET_TYPE::GRAPHICS_SHADER;
        break;
    }
    
    // T�� Ÿ���� �߸� ������ ���
    if (m_mapAsset->end() == m_mapAsset[(UINT)type].find(_strKey))
    {
        IF_FAILED(E_FAIL, "[CAssetMgr.h] �߸��� Ÿ���� ������ �߰��Ϸ��� �߽��ϴ�.");
        assert(nullptr);
    }
}

template<typename T>
inline T* CAssetMgr::FindAsset(const wstring& _strKey)
{
    return nullptr;
}
