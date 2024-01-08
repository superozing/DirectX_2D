#include "pch.h"
#include "CCameraMoveScript.h"

CCameraMoveScript::CCameraMoveScript()
	: m_CamSpeed(500.f)
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	// 카메라가 움직일 수 있도록 잠시 바꾸어요.
	if (KEY_PRESSED(KEY::W))
		vPos.y += DT * m_CamSpeed;

	if (KEY_PRESSED(KEY::S))
		vPos.y -= DT * m_CamSpeed;

	if (KEY_PRESSED(KEY::A))
		vPos.x -= DT * m_CamSpeed;

	if (KEY_PRESSED(KEY::D))
		vPos.x += DT * m_CamSpeed;

	if (KEY_PRESSED(KEY::_1))
	{
		if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
			Camera()->SetScale(Camera()->GetScale() + DT * 0.2f);
		else
			Camera()->SetFOV(Camera()->GetFOV() + DT * 2.f);
	}

	if (KEY_PRESSED(KEY::_2))
	{
		if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
			Camera()->SetScale(Camera()->GetScale() - DT * 0.2f);
		else
			Camera()->SetFOV(Camera()->GetFOV() - DT * 2.f);
	}



	Transform()->SetRelativePos(vPos);
}