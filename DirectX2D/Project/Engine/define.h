#pragma once

// type define
typedef void(*FUNC_TYPE)(void);


// func define
#define DEFINE constexpr auto 
#define SINGLE(classtype)  	private:\
								classtype();\
								~classtype();\
							friend class CSingleton<classtype>;

// const expr


// enum

