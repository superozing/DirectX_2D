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
	1. Device �ʱ�ȭ
	2. SwapChain ����
	3. Render Target�� View, Depth Stancil�� View ����
	4. Viewport ����
*/
int CDevice::init(HWND _hWnd, Vec2 _vResolution)
{
	// ��� ������(���� ������)
	m_hRenderWindow = _hWnd;

	// ���� �ػ�
	m_vRenderResolution = _vResolution;

	// Device(��ġ) �ʱ�ȭ
	D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL_11_0; // GPU�� �����ϴ� ��� ����

	IF_FAILED(D3D11CreateDevice(nullptr
				, D3D_DRIVER_TYPE_HARDWARE, nullptr
				, D3D11_CREATE_DEVICE_DEBUG
				, nullptr, 0, D3D11_SDK_VERSION
				, m_Device.GetAddressOf(), &eLevel, m_Context.GetAddressOf()), L"[CDevice.cpp] Device, Context ����")

	
	// ����ü�� ����
	IF_FAILED(CreateSwapChain(), L"[CDevice.cpp] SwapChain ����");

	// ���� Ÿ�ٰ� ���� Ÿ�� ��, ���� ���ٽ� Ÿ�ٰ� ���� ���ٽ� �� ���� 
	IF_FAILED(CreateTargetView(), L"[CDevice.cpp] Ÿ�ٰ� View ����");


	// RS, DS, Blend state ����
	IF_FAILED(CreateRasterizerState(),		L"Rasterizer State ����");
	IF_FAILED(CreateDepthStencilState(),	L"DepthStencil State ����");
	IF_FAILED(CreateBlendState(),			L"Blend State ����");
	
	// Viewport ����
	D3D11_VIEWPORT ViewportDesc{};

	// Viewport�� ��ġ�� ���� ����� x, y
	ViewportDesc.TopLeftX = 0; 
	ViewportDesc.TopLeftY = 0;

	// Viewport�� ���� ���� - ������ â �ػ󵵿� �����ϰ� ����.
	ViewportDesc.Width = m_vRenderResolution.x;
	ViewportDesc.Height = m_vRenderResolution.y;

	// ���� - ī�޶�κ��� �Ÿ� ����. 0(����� ����) ~ 1(�� ����)
	ViewportDesc.MinDepth = 0;	
	ViewportDesc.MaxDepth = 1.f;

	// Viewport Set
	CONTEXT->RSSetViewports(1, &ViewportDesc); // Viewport ������ ������ Viewport

	// ��� ���� ����
	IF_FAILED(CreateConstBuffer(), L"[CDevice.cpp] ��� ���� ����");

	return S_OK;
}

void CDevice::ClearView(float(&Color)[4])
{
	m_Context->ClearRenderTargetView(m_RTView.Get(), Color);
	m_Context->ClearDepthStencilView(m_DSView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CDevice::Present()
{
	m_SwapChain->Present(0, 0); // ���ڷ� 1. V-Sync ��� ����, 2. �ݵ�� 0���� ����(�⺻ ��)
}


// 1. ���� ü�� ���� ����
// 2. ���� ü�� ���丮 ����
// 3. ���� ü�� ����
int CDevice::CreateSwapChain()
{
	// SwapChain Description����
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// ������ ��, ����
	swapChainDesc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
	swapChainDesc.BufferDesc.Height = (UINT)m_vRenderResolution.y;

	// 32����Ʈ �� R�� 8, G�� 8, B�� 8, A�� 8��Ʈ ��ŭ ����ϰ�, 
	// 0���� 1������ �Ǽ��� ��Ÿ���� ����ȭ �� ���� ����
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 

	// �ֻ���
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;

	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;				  // ��� â ũ�Ⱑ �ٸ� ��� ����̹��� �˾Ƽ� ó��
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // ���� �׸��� ����� ����̹��� �˾Ƽ� ����
	
	// �÷���, �Ϲ������� 0
	swapChainDesc.Flags = 0; 

	// ��Ƽ ���ø� ��� ���ο� ����, ����Ƽ ����
	// ���� ���� �ε巯�� �ؽ��Ŀ� ȿ��
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	
	// â ���
	swapChainDesc.Windowed = true; 

	// SwapChain �� ��� ������ ����
	swapChainDesc.OutputWindow = m_hRenderWindow; 


	

	// ���� ü�� ���� ����� ������ �ִ� Factory�� ����
	ComPtr<IDXGIDevice>		pIdxgiDevice = nullptr; // �������̽� ���� �뵵
	ComPtr<IDXGIAdapter>	pAdapter = nullptr;		// �׷��� ī��(�����)Ư�� ���, ȭ�� �Ӽ� ��ȸ �뵵
	ComPtr<IDXGIFactory>	pFactory = nullptr;		// DXGI ��ü ���� - ���� SwapChain�� �����ϱ� ���ؼ� �����.

	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pIdxgiDevice.GetAddressOf());
	pIdxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

	// SwapChain ����
	IF_FAILED(pFactory->CreateSwapChain(m_Device.Get(), &swapChainDesc, m_SwapChain.GetAddressOf())
		, L"[CDevice.cpp, 129��] SwapChain ����")
	else
		return S_OK;
}

// 1. render target Texture�� SwapChain���κ��� ������
// 2. ���� �ؽ��ķ� render target view ����
//  ����
	//		ID3D11Resource
	//		|						 |
	//	ID3D11Buffer		 ID3D11Texture2D
	// 
	// view�� �ִ� �ֵ�
	// render target	(RT)
	// Depth Stencil	(DS)
	// Shader Resource	(SR)
	// Unordered Access (UA)
int CDevice::CreateTargetView()
{
	// Render Target Texture - SwapChain���Լ� ������. 
	// __uuidof�� �ش� Ŭ������ ĳ���� �ؼ� ���� ��� ��ȯ�ϵ��� ���ִ°ǰ�?
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_RTTex.GetAddressOf());

	// Render Target Texture ����
	m_Device->CreateRenderTargetView(m_RTTex.Get(), nullptr, m_RTView.GetAddressOf());

	// Depth Stencil Texture Description ����
	D3D11_TEXTURE2D_DESC depthStencilDesc{};

	// �ȼ� ����: Depth�� 3����Ʈ, Stencil�� 1����Ʈ ���
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Depth Stencil Texture�� �ػ󵵴� �ݵ�� Render Target Texture�� �����ϰ� ����
	depthStencilDesc.Width = (UINT)m_vRenderResolution.x;
	depthStencilDesc.Height = (UINT)m_vRenderResolution.y;

	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // �뵵�� Depth Stencil

	// CPU���� ���� �Ұ��� �ϵ��� ����
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

	// ���ø�
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	// ������Ƽ ������ �纻
	// ���� ũ��� render�� ū �̹����� �׸� �ʿ� ���� �̹� ������� ���� ũ���� �̹����� ����� ������ ����
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.MiscFlags = 0;

	// �ؽ��� �迭�� ũ�� - 1���� Ŭ ��� �迭�� �ȴ�.
	// ���⼭ �迭�� ������ ���� �� ũ���� ���� ���� �ؽ�ó�� �׷����� ���� ���� �ǹ�
	depthStencilDesc.ArraySize = 1;

	IF_FAILED(m_Device->CreateTexture2D(&depthStencilDesc, nullptr, m_DSTex.GetAddressOf()), L"[CDevice.cpp, 189��] depthStencilTexture ����");
	IF_FAILED(m_Device->CreateDepthStencilView(m_DSTex.Get(), nullptr, m_DSView.GetAddressOf()), L"[CDevice.cpp, 190��] depthStencilView ����");

	// OM(Output Merge State) �� RenderTargetTexture �� DepthStencilTexture ����
	m_Context->OMSetRenderTargets(1, m_RTView.GetAddressOf(), m_DSView.Get()); // �׷���...? �� RT�� ���� �����͸� �ѱ�鼭 DS�� �����͸� �ѱ���?
	
	return S_OK;
}

#pragma region D3D11_RASTERIZER_DESC ����
/*
FillMode: D3D11_FILL_MODE ����������, �ٰ����� ä�� ����� �����մϴ�. 
	���� ���, ���̾������� �������� �Ϸ��� D3D11_FILL_WIREFRAME���� �����մϴ�.

CullMode: D3D11_CULL_MODE ����������, � ���� �ø������� �����մϴ�. 
	������ �����δ� �ո鸸 �ø�(D3D11_CULL_FRONT) �Ǵ� �޸鸸 �ø�(D3D11_CULL_BACK), Ȥ�� �ø����� ����(D3D11_CULL_NONE) ���� �ֽ��ϴ�.

FrontCounterClockwise: BOOL Ÿ������, �ﰢ���� �������� �ð� �ݴ� �������� ���� ���θ� ��Ÿ���ϴ�. 
T	RUE�� �����ϸ� �ݽð� ������ �������� �Ǹ�, FALSE�� �����ϸ� �ð� ������ �������� �˴ϴ�.

DepthBias: ���� ������, ���� ���� �߰��� �������� ��Ÿ���ϴ�. 
	�̸� ���� Z-������ �����Ͽ� ���� �׽�Ʈ�� �Բ� �������� ��ü�� ���� ���� �浹�� ������ �� �ֽ��ϴ�.

DepthBiasClamp: �ε��Ҽ��� ������, 
	���� ���̾�� ������ ���� �ִ� ���Դϴ�.

SlopeScaledDepthBias: �ε��Ҽ��� ������, 
	�����Ͷ������� ���⿡ ���� ���� ���̾�� �����մϴ�.

DepthClipEnable: BOOL Ÿ������, 
	���� Ŭ������ Ȱ��ȭ���� ���θ� ��Ÿ���ϴ�.

ScissorEnable: BOOL Ÿ������, 
	���� �׽�Ʈ�� Ȱ��ȭ���� ���θ� ��Ÿ���ϴ�.

MultisampleEnable: BOOL Ÿ������, 
	��Ƽ���ø��� ������� ���θ� ��Ÿ���ϴ�.

AntialiasedLineEnable: BOOL Ÿ������, 
	���� ��Ƽ�ٸ������ ������� ���θ� ��Ÿ���ϴ�.
*/
#pragma endregion
int CDevice::CreateRasterizerState()
{
	// �⺻ ����
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

#pragma region D3D11_DEPTH_STENCIL_DESC ����
/*		DepthEnable: BOOL Ÿ������, ���� �׽�Ʈ�� ������� ���θ� ��Ÿ���ϴ�.
					TRUE�� �����ϸ� ���� �׽�Ʈ�� Ȱ��ȭ�Ǹ�, FALSE�� �����ϸ� ��Ȱ��ȭ�˴ϴ�.

		DepthWriteMask : D3D11_DEPTH_WRITE_MASK ����������, ���� ���� �� �� � ��Ʈ�� ��������� �����մϴ�.
					������ ������ D3D11_DEPTH_WRITE_MASK_ZERO(���� ���� ���� ����), D3D11_DEPTH_WRITE_MASK_ALL(��� ��Ʈ�� ���)�� �ֽ��ϴ�.

		DepthFunc : D3D11_COMPARISON_FUNC ����������, ���� �׽�Ʈ�� ����� �� �Լ��� �����մϴ�.
					���� ��� D3D11_COMPARISON_LESS�� ���� ���� ���� ���� ������ ������ ���� ���� ����ϵ��� �մϴ�.
*/
#pragma endregion
int CDevice::CreateDepthStencilState()
{
	HRESULT hr = S_OK;

	D3D11_DEPTH_STENCIL_DESC tDesc{};

	// �⺻ ����
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

	// No Write - ���� ����� ���� ����.
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

#pragma region D3D11_BLEND_DESC ����
/*
AlphaToCoverageEnable: BOOL Ÿ������, ���� �� Ŀ�������� Ȱ��ȭ���� ���θ� ��Ÿ���ϴ�. 
	�̰��� �ַ� ������ �ִ� ��ü�� �ټ� �������� �� ���Ǹ�, �پ��� ���� ���� ���� �ȼ��� �� ��Ȯ�ϰ� ������ �� �ֵ��� �մϴ�.

IndependentBlendEnable: BOOL Ÿ������, �� ���� Ÿ�ٿ� ���� �������� ���� ������ ������� ���θ� ��Ÿ���ϴ�. 
	FALSE�� �����ϸ� ù ��° ���� Ÿ���� ������ ��� ���� Ÿ�ٿ� ����Ǹ�, TRUE�� �����ϸ� �� ���� Ÿ�ٸ��� �ٸ� ���� ������ ����� �� �ֽ��ϴ�.

RenderTarget[8]: D3D11_RENDER_TARGET_BLEND_DESC �迭��, �ִ� 8���� ���� Ÿ�ٿ� ���� ���� ������ ��Ÿ���ϴ�. 
	�� ���� Ÿ�ٿ� ���� ������ D3D11_RENDER_TARGET_BLEND_DESC ����ü�� �����Ǿ� �ֽ��ϴ�.

			BlendEnable:			BOOL Ÿ������,			�ش� ���� Ÿ�ٿ� ������ ������� ���θ� ��Ÿ���ϴ�.
			SrcBlend:				D3D11_BLEND ����������,	�ҽ� ���� ���� ���͸� ��Ÿ���ϴ�.
			DestBlend:				D3D11_BLEND ����������,	��� ���� ���� ���͸� ��Ÿ���ϴ�.
			BlendOp:				D3D11_BLEND_OP ����������, ���� ������ ��Ÿ���ϴ�.
			SrcBlendAlpha:			D3D11_BLEND ����������,	�ҽ� ���� ���� ���͸� ��Ÿ���ϴ�.
			DestBlendAlpha:			D3D11_BLEND ����������,	��� ���� ���� ���͸� ��Ÿ���ϴ�.
			BlendOpAlpha:			D3D11_BLEND_OP ����������, ���� ���� ������ ��Ÿ���ϴ�.
			RenderTargetWriteMask:	��Ʈ����ũ��,				���� ���� ����ũ�� ��Ÿ���ϴ�.
*/
#pragma endregion
int CDevice::CreateBlendState()
{
	// �⺻ ����
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
	// Transform ��� ���� ����
	m_arrCB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer;
	m_arrCB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(tTransform), 1);

	// MATERIAL_CONST ��� ���� ����

	// GLOBAL_DATA ��� ���� ����
	
	// ANIMATION ��� ���� ����

	return S_OK;
}
