#pragma once

// ������Ʈ ���� �ֻ��� Ŭ���� Entity
class CEntity
{
private:
	static UINT g_NextID; // ID ���� �����Ű�鼭 ��ü�� ��������� ID���� ���� �Է��ؾ� �� �� �Է� ��Ű�� ++�� ������Ű��.

private:
	const UINT	m_ID;		// ID ��. ������ �ʾƾ� �ϱ� ������ const�� ����
	wstring		m_wstrName; // �̸�

public: // Get, Set
	void SetName(const wstring& _strName) { m_wstrName = _strName; }
	const wstring& GetName() { return m_wstrName; }

	UINT GetID() const { return m_ID; }

public:
	CEntity();
	virtual ~CEntity();
};

