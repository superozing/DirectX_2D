#pragma once


#pragma region _	Type Define

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

#pragma endregion
#pragma region _	Define - Function

#define DEFINE constexpr auto 

#define SINGLE(classtype)  	private:\
								classtype();\
								~classtype();\
							friend class CSingleton<classtype>;

#define INST(classtype) classtype::GetInst()
#define DEVICE  INST(CDevice)->GetDevice()
#define CONTEXT INST(CDevice)->GetContext()

#define IF_FAILED(Func, LstrFuncName) if (FAILED(Func))\
									{\
											wstring strFuncName = LstrFuncName;\
											strFuncName += L" 실패";\
											MessageBox(nullptr, strFuncName.c_str(), strFuncName.c_str(), MB_OK);\
											return E_FAIL;\
									}\

#define IF_DELETE(deletePtr) if (nullptr != deletePtr) { delete deletePtr; }

#define DT					INST(CTimeMgr)->GetDeltaTime()

#define KEY_CHECK(Key, State) INST(CKeyMgr)->GetKeyState(Key) == State
#define KEY_TAP(Key) KEY_CHECK(Key, TAP)
#define KEY_PRESSED(Key) KEY_CHECK(Key, PRESSED)
#define KEY_RELEASED(Key) KEY_CHECK(Key, RELEASED)
#define KEY_NONE(Key) KEY_CHECK(Key, NONE)

#pragma endregion
#pragma region _	Define - constexpr



#pragma endregion
#pragma region _	enum class
enum class ASSET_TYPE
{
	MESH,
	MESHDATA,
	TEXTURE,
	MATERIAL,
	SOUND,
	COMPUTE_SHADER,
	GRAPHICS_SHADER,
	END,
};

// 컴포넌트의 타입을 이제 인덱스를 사용해서 들고 있도록 해줌.
// 이러면 컴포넌트를 추가하는 순서에 상관 없이 원하는 인덱스로 바로바로 접근이 가능하다.
// 만약에 충돌체가 여러 개인 경우는 어떻게 해야 할까?
enum class COMPONENT_TYPE
{
	TRANSFORM,	// 오브젝트의 위치,크기,회전

	COLLIDER2D,	// 2차원 충돌체
	COLLIDER3D, // 3차원 충돌체

	ANIMATOR2D,	// 스프라이트 Animation
	ANIMATOR3D,	// Bone Skinning Animation

	LIGHT2D,	// 2차원 광원
	LIGHT3D,	// 3차원 광원

	CAMERA,		// 카메라 기능



	// 렌더링 기능 관련 컴포넌트
	MESHRENDER, // 매쉬 렌더
	TILEMAP,	// 타입맵
	PARTICLESYSTEM, // 파티클 시스템 (여기까지가 2D까지 만들 컴포넌트) 
	SKYBOX,		// 스카이박스
	DECAL,		// 데칼
	LANDSCAPE,

	END,

	SCRIPT,
};

// 방향 정보를 나타내는 enum class 
// 음수를 가리키면 각자 반대의 방향을 가리킬 수 있다.
// 총 x, y, z축 6방향을 가리킬 수 있다.
enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,

	END,
};


// 상수버퍼 종류
enum class CB_TYPE
{
	TRANSFORM,		// 트랜스폼
	MATERIAL_CONST, // 상수 머터리얼
	GLOBAL_DATA,	// GD
	ANIMATION,		// animation

	END,
};

#pragma endregion

