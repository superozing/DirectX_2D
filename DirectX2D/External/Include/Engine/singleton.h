#pragma once

// 싱글턴 템플릿 클래스를 상속 시
// 자신 클래스 명을 넣으면 그에 맞는 싱글턴 기능을 받아가요.
template <typename T>
class CSingleton
{
private:
	static T* m_This;

public: // 공개 함수

	// 처음 호출 시에만 객체를 생성하고, 이후에는 생성된 객체를 반환한다.
	static T* GetInst()
	{
		if (nullptr == m_This)
			m_This = new T;
	
		return m_This;
	}

	// 객체 생성 시에만 delete를 해주도록 Destroy() 함수를 만들어요.
	static void Destroy()
	{
		if (nullptr == m_This)
			delete m_This;
		m_This = nullptr;
	}

public: // 생성자, 소멸자

	CSingleton()
	{
		atexit((FUNC_TYPE)(&CSingleton::Destroy)); // 프로그램 종료 시 동작 -> 알아서 객체를 삭제하도록 해주어요.
	}

	~CSingleton() {}
};

// static은 실체가 없기에 어딘가에서 정의해주어야 한다.
template <typename T>
T* CSingleton<T>::m_This = nullptr; 