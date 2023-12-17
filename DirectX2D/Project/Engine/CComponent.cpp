#include "pch.h"
#include "CComponent.h"



// 기본 생성자를 정의하지 않음. 반드시 컴포넌트 타입을 인자로 받도록
CComponent::CComponent(COMPONENT_TYPE _type)
	:m_Type(_type)
{
}

CComponent::~CComponent()
{
}
