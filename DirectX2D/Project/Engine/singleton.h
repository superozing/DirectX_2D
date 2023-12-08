#pragma once

// �̱��� ���ø� Ŭ������ ��� ��
// �ڽ� Ŭ���� ���� ������ �׿� �´� �̱��� ����� �޾ư���.
template <typename T>
class CSingleton
{
private:
	static T* m_This;

public: // ���� �Լ�

	// ó�� ȣ�� �ÿ��� ��ü�� �����ϰ�, ���Ŀ��� ������ ��ü�� ��ȯ�Ѵ�.
	static T* GetInst()
	{
		if (nullptr == m_This)
			m_This = new T;
	
		return m_This;
	}

	// ��ü ���� �ÿ��� delete�� ���ֵ��� Destroy() �Լ��� ������.
	static void Destroy()
	{
		if (nullptr == m_This)
			delete m_This;
		m_This = nullptr;
	}

public: // ������, �Ҹ���

	CSingleton()
	{
		atexit((FUNC_TYPE)(&CSingleton::Destroy)); // ���α׷� ���� �� ���� -> �˾Ƽ� ��ü�� �����ϵ��� ���־��.
	}

	~CSingleton() {}
};

// static�� ��ü�� ���⿡ ��򰡿��� �������־�� �Ѵ�.
template <typename T>
T* CSingleton<T>::m_This = nullptr; 