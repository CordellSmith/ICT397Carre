#pragma once

template<class T>
class Singleton
{
public:
	Static T* GetInstance() {
		static T instance;
		
		return &instance;
	}

private:
	Singleton();
	~Singleton();

	// Hidden
	Singleton(const Singleton &);
	Singleton &operator=(const Singleton &);
};