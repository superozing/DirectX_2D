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
											strFuncName += L" ����";\
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

// ������Ʈ�� Ÿ���� ���� �ε����� ����ؼ� ��� �ֵ��� ����.
// �̷��� ������Ʈ�� �߰��ϴ� ������ ��� ���� ���ϴ� �ε����� �ٷιٷ� ������ �����ϴ�.
// ���࿡ �浹ü�� ���� ���� ���� ��� �ؾ� �ұ�?
enum class COMPONENT_TYPE
{
	TRANSFORM,	// ������Ʈ�� ��ġ,ũ��,ȸ��

	COLLIDER2D,	// 2���� �浹ü
	COLLIDER3D, // 3���� �浹ü

	ANIMATOR2D,	// ��������Ʈ Animation
	ANIMATOR3D,	// Bone Skinning Animation

	LIGHT2D,	// 2���� ����
	LIGHT3D,	// 3���� ����

	CAMERA,		// ī�޶� ���



	// ������ ��� ���� ������Ʈ
	MESHRENDER, // �Ž� ����
	TILEMAP,	// Ÿ�Ը�
	PARTICLESYSTEM, // ��ƼŬ �ý��� (��������� 2D���� ���� ������Ʈ) 
	SKYBOX,		// ��ī�̹ڽ�
	DECAL,		// ��Į
	LANDSCAPE,

	END,

	SCRIPT,
};

// ���� ������ ��Ÿ���� enum class 
// ������ ����Ű�� ���� �ݴ��� ������ ����ų �� �ִ�.
// �� x, y, z�� 6������ ����ų �� �ִ�.
enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,

	END,
};


// ������� ����
enum class CB_TYPE
{
	TRANSFORM,		// Ʈ������
	MATERIAL_CONST, // ��� ���͸���
	GLOBAL_DATA,	// GD
	ANIMATION,		// animation

	END,
};

#pragma endregion

