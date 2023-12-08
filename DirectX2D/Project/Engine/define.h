#pragma once


#pragma region _	Type Define

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

typedef void(*FUNC_TYPE)(void);

#pragma endregion
#pragma region _	Define - Function

#define DEFINE constexpr auto 

#define SINGLE(classtype)  	private:\
								classtype();\
								~classtype();\
							friend class CSingleton<classtype>;

#define INST(classtype) classtype::GetInst()

#define IF_FAILED(Func, LstrFuncName) if (FAILED(Func))\
									{\
											wstring strFuncName = LstrFuncName;\
											strFuncName += L" ½ÇÆÐ";\
											MessageBox(nullptr, strFuncName.c_str(), strFuncName.c_str(), MB_OK);\
											return E_FAIL;\
									}\



#pragma endregion
#pragma region _	Define - constexpr



#pragma endregion
#pragma region _	enum class



#pragma endregion

