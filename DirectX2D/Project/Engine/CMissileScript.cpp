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
	// 소유 오브젝트로부터 트랜스폼 컴포넌트로 접근, Pos를 가져옴.
	Vec3 vPos = Transform()->GetRelativePos();

	// y 값을 지속적으로 증가시킴
	vPos.y += m_fSpeed * DT;

	// 소유 오브젝트로부터 트랜스폼 컴포넌트로 접근, Pos를 설정함.
	Transform()->SetRelativePos(vPos);
}