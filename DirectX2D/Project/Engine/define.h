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
											strFuncName += L" ½ÇÆÐ";\
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
};

#pragma endregion

