#include "pch.h"
#include "CDevice.h"

#include "CConstBuffer.h"

CDevice::CDevice()
	: m_hRenderWindow(nullptr)
	, m_arrCB{}
	, m_arrRS{}
	, m_arrDS{}
	, m_arrBS{}
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

	IF_FAILED(D3D11CreateDevice(nullptr
				, D3D_DRIVER_TYPE_HARDWARE, nullptr
				, D3D11_CREATE_DEVICE_DEBUG
				, nullptr, 0, D3D11_SDK_VERSION
				, m_Device.GetAddressOf(), &eLevel, m_Context.GetAddressOf()), L"[CDevice.cpp] Device, Context 생성")

	
	// 스왑체인 생성
	IF_FAILED(CreateSwapChain(), L"[CDevice.cpp] SwapChain 생성");

	// 렌더 타겟과 렌더 타겟 뷰, 뎁스 스텐실 타겟과 뎁스 스텐실 뷰 생성 
	IF_FAILED(CreateTargetView(), L"[CDevice.cpp] 타겟과 View 생성");


	// RS, DS, Blend state 생성
	IF_FAILED(CreateRasterizerState(),		L"Rasterizer State 생성");
	IF_FAILED(CreateDepthStencilState(),	L"DepthStencil State 생성");
	IF_FAILED(CreateBlendState(),			L"Blend State 생성");
	
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

#pragma region D3D11_RASTERIZER_DESC 설명
/*
FillMode: D3D11_FILL_MODE 열거형으로, 다각형의 채움 방식을 지정합니다. 
	예를 들어, 와이어프레임 렌더링을 하려면 D3D11_FILL_WIREFRAME으로 설정합니다.

CullMode: D3D11_CULL_MODE 열거형으로, 어떤 면을 컬링할지를 지정합니다. 
	가능한 값으로는 앞면만 컬링(D3D11_CULL_FRONT) 또는 뒷면만 컬링(D3D11_CULL_BACK), 혹은 컬링하지 않음(D3D11_CULL_NONE) 등이 있습니다.

FrontCounterClockwise: BOOL 타입으로, 삼각형의 정방향을 시계 반대 방향으로 할지 여부를 나타냅니다. 
T	RUE로 설정하면 반시계 방향이 정방향이 되며, FALSE로 설정하면 시계 방향이 정방향이 됩니다.

DepthBias: 정수 값으로, 깊이 값에 추가할 오프셋을 나타냅니다. 
	이를 통해 Z-편차를 조절하여 깊이 테스트와 함께 렌더링된 물체들 간의 깊이 충돌을 방지할 수 있습니다.

DepthBiasClamp: 부동소수점 값으로, 
	깊이 바이어스를 적용할 때의 최대 값입니다.

SlopeScaledDepthBias: 부동소수점 값으로, 
	레스터라이저의 기울기에 따라 깊이 바이어스를 조절합니다.

DepthClipEnable: BOOL 타입으로, 
	깊이 클리핑을 활성화할지 여부를 나타냅니다.

ScissorEnable: BOOL 타입으로, 
	시저 테스트를 활성화할지 여부를 나타냅니다.

MultisampleEnable: BOOL 타입으로, 
	멀티샘플링을 사용할지 여부를 나타냅니다.

AntialiasedLineEnable: BOOL 타입으로, 
	선의 안티앨리어싱을 사용할지 여부를 나타냅니다.
*/
#pragma endregion
int CDevice::CreateRasterizerState()
{
	// 기본 세팅
	m_arrRS[(UINT)RS_TYPE::CULL_BACK] = nullptr;

	HRESULT hr = S_OK;

	D3D11_RASTERIZER_DESC tDesc{};

	tDesc.CullMode = D3D11_CULL_FRONT;
	tDesc.FillMode = D3D11_FILL_SOLID;
	hr = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());
	IF_FAILED(hr, L"[CDevice.cpp]CreateRasterizerState() - CULL_FRONT");

	tDesc.CullMode = D3D11_CULL_NONE;
	tDesc.FillMode = D3D11_FILL_SOLID;
	hr = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());
	IF_FAILED(hr, L"[CDevice.cpp]CreateRasterizerState() - CULL_NONE");


	tDesc.CullMode = D3D11_CULL_NONE;
	tDesc.FillMode = D3D11_FILL_WIREFRAME;
	hr = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());
	IF_FAILED(hr, L"[CDevice.cpp]CreateRasterizerState() - WIRE_FRAME");

	return S_OK;
}

#pragma region D3D11_DEPTH_STENCIL_DESC 설명
/*		DepthEnable: BOOL 타입으로, 깊이 테스트를 사용할지 여부를 나타냅니다.
					TRUE로 설정하면 깊이 테스트가 활성화되며, FALSE로 설정하면 비활성화됩니다.

		DepthWriteMask : D3D11_DEPTH_WRITE_MASK 열거형으로, 깊이 값을 쓸 때 어떤 비트를 사용할지를 지정합니다.
					가능한 값으로 D3D11_DEPTH_WRITE_MASK_ZERO(깊이 값을 쓰지 않음), D3D11_DEPTH_WRITE_MASK_ALL(모든 비트를 사용)이 있습니다.

		DepthFunc : D3D11_COMPARISON_FUNC 열거형으로, 깊이 테스트에 사용할 비교 함수를 지정합니다.
					예를 들어 D3D11_COMPARISON_LESS는 깊이 값이 현재 깊이 버퍼의 값보다 작을 때만 통과하도록 합니다.
*/
#pragma endregion
int CDevice::CreateDepthStencilState()
{
	HRESULT hr = S_OK;

	D3D11_DEPTH_STENCIL_DESC tDesc{};

	// 기본 세팅
	m_arrDS[(UINT)DS_TYPE::LESS] = nullptr;

	// Less Equal
	tDesc.DepthEnable = true;
	tDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable = false;

	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());
	IF_FAILED(hr, L"[CDevice.cpp] CreateDepthStencilState() - LESS_EQUAL");


	// Greater
	tDesc.DepthEnable = true;
	tDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable = false;
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::GRATER].GetAddressOf());
	IF_FAILED(hr, L"[CDevice.cpp] CreateDepthStencilState() - GRATER");

	// Greater Equal
	tDesc.DepthEnable = true;
	tDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable = false;
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::GRATER_EQUAL].GetAddressOf());
	IF_FAILED(hr, L"[CDevice.cpp] CreateDepthStencilState() - GRATER_EQUAL");

	// No Test
	tDesc.DepthEnable = false;
	tDesc.DepthFunc = D3D11_COMPARISON_NEVER;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable = false;
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_TEST].GetAddressOf());
	IF_FAILED(hr, L"[CDevice.cpp] CreateDepthStencilState() - NO_TEST");

	// No Write - 깊이 기록을 하지 않음.
	tDesc.DepthEnable = true;
	tDesc.DepthFunc = D3D11_COMPARISON_LESS;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	tDesc.StencilEnable = false;
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());
	IF_FAILED(hr, L"[CDevice.cpp] CreateDepthStencilState() - NO_WRITE");

	// NoTest NoWrite
	tDesc.DepthEnable = false;
	tDesc.DepthFunc = D3D11_COMPARISON_NEVER;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	tDesc.StencilEnable = false;
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());
	IF_FAILED(hr, L"[CDevice.cpp] CreateDepthStencilState() - NO_TEST_NO_WRITE");

	return S_OK;
}

#pragma region D3D11_BLEND_DESC 설명
/*
AlphaToCoverageEnable: BOOL 타입으로, 알파 투 커버리지를 활성화할지 여부를 나타냅니다. 
	이것은 주로 투명도가 있는 객체를 다수 렌더링할 때 사용되며, 다양한 알파 값을 가진 픽셀을 더 정확하게 블렌딩할 수 있도록 합니다.

IndependentBlendEnable: BOOL 타입으로, 각 렌더 타겟에 대해 독립적인 블렌딩 설정을 사용할지 여부를 나타냅니다. 
	FALSE로 설정하면 첫 번째 렌더 타겟의 설정이 모든 렌더 타겟에 적용되며, TRUE로 설정하면 각 렌더 타겟마다 다른 블렌딩 설정을 사용할 수 있습니다.

RenderTarget[8]: D3D11_RENDER_TARGET_BLEND_DESC 배열로, 최대 8개의 렌더 타겟에 대한 블렌딩 설정을 나타냅니다. 
	각 렌더 타겟에 대한 설정은 D3D11_RENDER_TARGET_BLEND_DESC 구조체로 구성되어 있습니다.

			BlendEnable:			BOOL 타입으로,			해당 렌더 타겟에 블렌딩을 사용할지 여부를 나타냅니다.
			SrcBlend:				D3D11_BLEND 열거형으로,	소스 색상 블렌딩 팩터를 나타냅니다.
			DestBlend:				D3D11_BLEND 열거형으로,	대상 색상 블렌딩 팩터를 나타냅니다.
			BlendOp:				D3D11_BLEND_OP 열거형으로, 블렌딩 연산을 나타냅니다.
			SrcBlendAlpha:			D3D11_BLEND 열거형으로,	소스 알파 블렌딩 팩터를 나타냅니다.
			DestBlendAlpha:			D3D11_BLEND 열거형으로,	대상 알파 블렌딩 팩터를 나타냅니다.
			BlendOpAlpha:			D3D11_BLEND_OP 열거형으로, 알파 블렌딩 연산을 나타냅니다.
			RenderTargetWriteMask:	비트마스크로,				색상 쓰기 마스크를 나타냅니다.
*/
#pragma endregion
int CDevice::CreateBlendState()
{
	// 기본 세팅
	m_arrBS[(UINT)BS_TYPE::DEFAULT] = nullptr;

	D3D11_BLEND_DESC tDesc{};

	// AlphaBlend
	tDesc.AlphaToCoverageEnable = false;
	tDesc.IndependentBlendEnable = true;

	tDesc.RenderTarget[0].BlendEnable = true;
	tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	DEVICE->CreateBlendState(&tDesc, m_arrBS[(UINT)BS_TYPE::ALPHA_BLEND].GetAddressOf());

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
