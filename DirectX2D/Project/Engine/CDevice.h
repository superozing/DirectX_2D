#pragma once
#include "singleton.h"

class CConstBuffer;


// 엔진 라이브러리의 가장 대표가 되는 싱글턴 객체.
// DirectX 라이브러리를 사용해서 GPU 관련 작업을 진행해요.
class CDevice :
    public CSingleton<CDevice>
{
	SINGLE(CDevice);
private: // ComPtr: DirectX의 스마트 포인터
    ComPtr<ID3D11Device>           m_Device;    // GPU 메모리 관리, 객체 생성
    ComPtr<ID3D11DeviceContext>    m_Context;   // GPU 렌더링 명령

	ComPtr<IDXGISwapChain>			m_SwapChain; // 스왑체인 - 출력 버퍼 지정

	// OM - Output Merge State
	ComPtr<ID3D11Texture2D>			m_RTTex;     // 렌더타겟 텍스쳐
	ComPtr<ID3D11RenderTargetView>	m_RTView;    // 렌더타겟 뷰

	ComPtr<ID3D11Texture2D>			m_DSTex;	 // 뎁스 스텐실 텍스쳐	
	ComPtr<ID3D11DepthStencilView>	m_DSView;	 // 뎁스 스텐실 뷰

	HWND							m_hRenderWindow;
	Vec2							m_vRenderResolution;

	// 상수 버퍼들
	CConstBuffer* m_arrCB[(UINT)CB_TYPE::END];

	ComPtr<ID3D11RasterizerState>	m_arrRS[(UINT)RS_TYPE::END];
	ComPtr<ID3D11DepthStencilState>	m_arrDS[(UINT)DS_TYPE::END];
	ComPtr<ID3D11BlendState>		m_arrBS[(UINT)BS_TYPE::END];

public: // Get, Set
	ID3D11Device*			GetDevice()	 { return m_Device.Get(); }
	ID3D11DeviceContext*	GetContext() { return m_Context.Get(); }

	CConstBuffer*			GetConstBuffer(CB_TYPE _type) const { return m_arrCB[(UINT)_type]; }
	Vec2					GetRenderResolution() const			{ return m_vRenderResolution; }


	ComPtr<ID3D11RasterizerState>	GetRSState(RS_TYPE _Type) const { return m_arrRS[(UINT)_Type]; }
	ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) const { return m_arrDS[(UINT)_Type]; }
	ComPtr<ID3D11BlendState>		GetBSState(BS_TYPE _Type) const { return m_arrBS[(UINT)_Type]; }

public:
	int init(HWND _hWnd, Vec2 _vResolution);

	// 렌더 타겟, 뎁스 스텐실 클리어
	void ClearView(float(&Color)[4]); 

	// main window로 보내기
	void Present();


private:
	int CreateSwapChain();
	int CreateTargetView();

	int CreateRasterizerState();
	int CreateDepthStencilState();
	int CreateBlendState();

	int CreateConstBuffer();
};

