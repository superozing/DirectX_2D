#include "pch.h"
#include "CMissileScript.h"


CMissileScript::CMissileScript()
	: m_fSpeed(1500.f)
{
}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::tick()
{
	// ���� ������Ʈ�κ��� Ʈ������ ������Ʈ�� ����, Pos�� ������.
	Vec3 vPos = Transform()->GetRelativePos();

	// y ���� ���������� ������Ŵ
	vPos.y += m_fSpeed * DT;

	// ���� ������Ʈ�κ��� Ʈ������ ������Ʈ�� ����, Pos�� ������.
	Transform()->SetRelativePos(vPos);
}