#pragma once

#pragma region _	include 

#include <stdlib.h>
#include <Windows.h>

#include <string>

// container
#include <vector>
#include <list>
#include <map>

// wrl - ComPtr
#include <wrl.h>

// DirectX11 헤더 및 라이브러리 참조
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

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

// 엔진 헤더 참조
#pragma region _	Engine Header include

#include "define.h"
#include "singleton.h"
#include "struct.h"

#pragma endregion
