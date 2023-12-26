#pragma once

// 오브젝트 들의 최상위 클래스 Entity
class CEntity
{
private:
	static UINT g_NextID; // ID 값을 저장시키면서 객체가 만들어져서 ID값을 새로 입력해야 할 때 입력 시키고 ++로 증가시키기.

private:
	const UINT	m_ID;		// ID 값. 변하지 않아야 하기 때문에 const로 설정
	wstring		m_wstrName; // 이름

public: // Get, Set
	void SetName(const wstring& _strName) { m_wstrName = _strName; }
	const wstring& GetName() { return m_wstrName; }

	UINT GetID() const { return m_ID; }

public:
	CEntity();
	virtual ~CEntity();
};

