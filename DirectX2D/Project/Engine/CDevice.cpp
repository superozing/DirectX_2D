#include "pch.h"
#include "CDevice.h"

#include "CConstBuffer.h"

CDevice::CDevice()
	: m_hRenderWindow(nullptr)
	, m_arrCB{}
{
}

CDevice::~CDevice()
{
	Delete_Array(m_arrCB);
}

/*
* CDevice::init
	1. Device 초기화
	2. SwapChain 생성
	3. Render Target과 View, Depth Stancil과 View 생성
	4. Viewport 설정
*/
int CDevice::init(HWND _hWnd, Vec2 _vResolution)
{
	// 출력 윈도우(메인 윈도우)
	m_hRenderWindow = _hWnd;

	// 버퍼 해상도
	m_vRenderResolution = _vResolution;

	// Device(장치) 초기화
	D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL_11_0; // GPU가 지원하는 기능 수준

	if (FAILED(D3D11CreateDevice(nullptr
		, D3D_DRIVER_TYPE_HARDWARE, nullptr
		, D3D11_CREATE_DEVICE_DEBUG
		, nullptr, 0, D3D11_SDK_VERSION
		, m_Device.GetAddressOf(), &eLevel, m_Context.GetAddressOf())))
	{
		MessageBox(nullptr, L"Device, Context 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}		
	
	// 스왑체인 생성
	IF_FAILED(CreateSwapChain(), L"[CDevice.cpp] SwapChain 생성");

	// 렌더 타겟과 렌더 타겟 뷰, 뎁스 스텐실 타겟과 뎁스 스텐실 뷰 생성 
	IF_FAILED(CreateTargetView(), L"[CDevice.cpp] 타겟과 View 생성");

	
	// Viewport 생성
	D3D11_VIEWPORT ViewportDesc{};

	// Viewport가 위치할 왼쪽 상단의 x, y
	ViewportDesc.TopLeftX = 0; 
	ViewportDesc.TopLeftY = 0;

	// Viewport의 폭과 높이 - 윈도우 창 해상도와 동일하게 맟춤.
	ViewportDesc.Width = m_vRenderResolution.x;
	ViewportDesc.Height = m_vRenderResolution.y;

	// 깊이 - 카메라로부터 거리 정보. 0(가까운 깊이) ~ 1(먼 깊이)
	ViewportDesc.MinDepth = 0;	
	ViewportDesc.MaxDepth = 1.f;

	// Viewport Set
	CONTEXT->RSSetViewports(1, &ViewportDesc); // Viewport 개수와 설정할 Viewport

	// 상수 버퍼 생성
	IF_FAILED(CreateConstBuffer(), L"[CDevice.cpp] 상수 버퍼 생성");

	return S_OK;
}

void CDevice::ClearView(float(&Color)[4])
{
	m_Context->ClearRenderTargetView(m_RTView.Get(), Color);
	m_Context->ClearDepthStencilView(m_DSView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CDevice::Present()
{
	m_SwapChain->Present(0, 0); // 인자로 1. V-Sync 사용 여부, 2. 반드시 0으로 설정(기본 값)
}


// 1. 스왑 체인 정보 생성
// 2. 스왑 체인 팩토리 접근
// 3. 스왑 체인 생성
int CDevice::CreateSwapChain()
{
	// SwapChain Description생성
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// 버퍼의 폭, 높이
	swapChainDesc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
	swapChainDesc.BufferDesc.Height = (UINT)m_vRenderResolution.y;

	// 32바이트 중 R을 8, G를 8, B를 8, A를 8비트 만큼 사용하고, 
	// 0에서 1사이의 실수로 나타내는 정규화 된 색상 포멧
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 

	// 주사율
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;

	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;				  // 출력 창 크기가 다를 경우 드라이버가 알아서 처리
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // 줄을 그리는 방법을 드라이버가 알아서 선택
	
	// 플래그, 일반적으로 0
	swapChainDesc.Flags = 0; 

	// 멀티 샘플링 사용 여부와 개수, 퀄리티 설정
	// 많을 수록 부드러운 텍스쳐와 효과
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	
	// 창 모드
	swapChainDesc.Windowed = true; 

	// SwapChain 의 출력 윈도우 지정
	swapChainDesc.OutputWindow = m_hRenderWindow; 


	

	// 스왑 체인 생성 기능을 가지고 있는 Factory에 접근
	ComPtr<IDXGIDevice>		pIdxgiDevice = nullptr; // 인터페이스 접근 용도
	ComPtr<IDXGIAdapter>	pAdapter = nullptr;		// 그래픽 카드(어답터)특성 얻기, 화면 속성 조회 용도
	ComPtr<IDXGIFactory>	pFactory = nullptr;		// DXGI 객체 생성 - 보통 SwapChain을 생성하기 위해서 사용함.

	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pIdxgiDevice.GetAddressOf());
	pIdxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

	// SwapChain 생성
	IF_FAILED(pFactory->CreateSwapChain(m_Device.Get(), &swapChainDesc, m_SwapChain.GetAddressOf())
		, L"[CDevice.cpp, 129줄] SwapChain 생성")
	else
		return S_OK;
}

// 1. render target Texture를 SwapChain으로부터 얻어오기
// 2. 얻어온 텍스쳐로 render target view 생성
//  구조
	//		ID3D11Resource
	//		|						 |
	//	ID3D11Buffer		 ID3D11Texture2D
	// 
	// view가 있는 애들
	// render target	(RT)
	// Depth Stencil	(DS)
	// Shader Resource	(SR)
	// Unordered Access (UA)
int CDevice::CreateTargetView()
{
	// Render Target Texture - SwapChain에게서 얻어오기. 
	// __uuidof는 해당 클래스로 캐스팅 해서 맞을 경우 반환하도록 해주는건가?
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_RTTex.GetAddressOf());

	// Render Target Texture 생성
	m_Device->CreateRenderTargetView(m_RTTex.Get(), nullptr, m_RTView.GetAddressOf());

	// Depth Stencil Texture Description 생성
	D3D11_TEXTURE2D_DESC depthStencilDesc{};

	// 픽셀 포멧: Depth가 3바이트, Stencil이 1바이트 사용
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Depth Stencil Texture의 해상도는 반드시 Render Target Texture와 동일하게 설정
	depthStencilDesc.Width = (UINT)m_vRenderResolution.x;
	depthStencilDesc.Height = (UINT)m_vRenderResolution.y;

	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // 용도가 Depth Stencil

	// CPU에서 접근 불가능 하도록 설정
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

	// 샘플링
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	// 저퀄리티 버전의 사본
	// 작은 크기로 render시 큰 이미지로 그릴 필요 없이 이미 만들어진 작은 크기의 이미지를 사용할 것인지 여부
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.MiscFlags = 0;

	// 텍스쳐 배열의 크기 - 1보다 클 경우 배열이 된다.
	// 여기서 배열은 동일한 형식 및 크기의 여러 개의 텍스처를 그룹으로 묶은 것을 의미
	depthStencilDesc.ArraySize = 1;

	IF_FAILED(m_Device->CreateTexture2D(&depthStencilDesc, nullptr, m_DSTex.GetAddressOf()), L"[CDevice.cpp, 189줄] depthStencilTexture 생성");
	IF_FAILED(m_Device->CreateDepthStencilView(m_DSTex.Get(), nullptr, m_DSView.GetAddressOf()), L"[CDevice.cpp, 190줄] depthStencilView 생성");

	// OM(Output Merge State) 에 RenderTargetTexture 와 DepthStencilTexture 세팅
	m_Context->OMSetRenderTargets(1, m_RTView.GetAddressOf(), m_DSView.Get()); // 그러게...? 왜 RT는 이중 포인터를 넘기면서 DS는 포인터를 넘기지?
	
	return S_OK;
}

int CDevice::CreateConstBuffer()
{
	// Transform 상수 버퍼 생성
	m_arrCB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer;
	m_arrCB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(tTransform), 1);

	// MATERIAL_CONST 상수 버퍼 생성

	// GLOBAL_DATA 상수 버퍼 생성
	
	// ANIMATION 상수 버퍼 생성

	return S_OK;
}
