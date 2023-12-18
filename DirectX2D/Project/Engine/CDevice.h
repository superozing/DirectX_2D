#pragma once
#include "singleton.h"

class CConstBuffer;


// ���� ���̺귯���� ���� ��ǥ�� �Ǵ� �̱��� ��ü.
// DirectX ���̺귯���� ����ؼ� GPU ���� �۾��� �����ؿ�.
class CDevice :
    public CSingleton<CDevice>
{
	SINGLE(CDevice);
private: // ComPtr: DirectX�� ����Ʈ ������
    ComPtr<ID3D11Device>           m_Device;    // GPU �޸� ����, ��ü ����
    ComPtr<ID3D11DeviceContext>    m_Context;   // GPU ������ ���

	ComPtr<IDXGISwapChain>			m_SwapChain; // ����ü�� - ��� ���� ����

	// OM - Output Merge State
	ComPtr<ID3D11Texture2D>			m_RTTex;     // ����Ÿ�� �ؽ���
	ComPtr<ID3D11RenderTargetView>	m_RTView;    // ����Ÿ�� ��

	ComPtr<ID3D11Texture2D>			m_DSTex;	 // ���� ���ٽ� �ؽ���	
	ComPtr<ID3D11DepthStencilView>	m_DSView;	 // ���� ���ٽ� ��

	HWND							m_hRenderWindow;
	Vec2							m_vRenderResolution;

	// ��� ���۵�
	CConstBuffer* m_arrCB[(UINT)CB_TYPE::END];


public: // Get, Set
	ID3D11Device*			GetDevice()	 { return m_Device.Get(); }
	ID3D11DeviceContext*	GetContext() { return m_Context.Get(); }

	CConstBuffer* GetConstBuffer(CB_TYPE _type) const { return m_arrCB[(UINT)_type]; }

public:
	int init(HWND _hWnd, Vec2 _vResolution);

	// ���� Ÿ��, ���� ���ٽ� Ŭ����
	void ClearView(float(&Color)[4]); 

	// main window�� ������
	void Present();


private:
	int CreateSwapChain();
	int CreateTargetView();
	int CreateConstBuffer();
};

