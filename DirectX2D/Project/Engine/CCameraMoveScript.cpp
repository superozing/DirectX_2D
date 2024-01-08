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

	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	if (KEY_PRESSED(KEY::W)) vPos += DT * m_CamSpeed * vFront;
	if (KEY_PRESSED(KEY::S)) vPos += DT * m_CamSpeed * -vFront;
	if (KEY_PRESSED(KEY::A)) vPos += DT * m_CamSpeed * -vRight;
	if (KEY_PRESSED(KEY::D)) vPos += DT * m_CamSpeed * vRight;

	Transform()->SetRelativePos(vPos);

	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 vDrag = INST(CKeyMgr)->GetMouseDrag();
		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.y += vDrag.x * DT * XM_PI * 4.f;
		vRot.x += vDrag.y * DT * XM_PI * 4.f;
		Transform()->SetRelativeRotation(vRot);
	}
}
