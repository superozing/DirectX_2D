#pragma once

#pragma region _	include 

#include <stdlib.h>
#include <Windows.h>

#include <typeinfo>

#include <string>

// container
#include <vector>
#include <list>
#include <map>

// wrl - ComPtr
#include <wrl.h>

// DirectX11 ��� �� ���̺귯�� ����
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXTex/DirectXTex.h>
#ifdef _DEBUG
#pragma commment(lib, "DirectXTex\\DirectXTex_debug")
#else
#pragma commment(lib, "DirectXTex\\DirectXTex")
#endif

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// simpleMath
#include "SimpleMath.h"

#pragma endregion


#pragma region _	using

using std::wstring;
using std::string;

// container
using std::vector;
using std::list;
using std::map;
using std::make_pair;

// wrl
using namespace Microsoft::WRL;

// DirectX11
using namespace DirectX;

// SimpleMath
using namespace DirectX::SimpleMath;

#pragma endregion

// ���� ��� ����
#pragma region _	Engine Header include

#include "singleton.h"
#include "define.h"
#include "struct.h"
#include "Func.h"

#pragma endregion
