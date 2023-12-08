#pragma once


#pragma region _	Type Define

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

typedef void(*FUNC_TYPE)(void);


// func define
#define DEFINE constexpr auto 
#define SINGLE(classtype)  	private:\
								classtype();\
								~classtype();\
							friend class CSingleton<classtype>;

// const expr


// enum

