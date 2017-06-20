#pragma once

#include <QMutex>
#include <QScopedPointer>

//����ģʽ������


//����
//==================================================================================================
template<typename T>
class SingletonHelper 
{
public:
	static T * getInstance();

private:
	static QMutex mutex;
	static QScopedPointer<T> instance;
};


//ʵ��
//==================================================================================================
template<typename T> QMutex SingletonHelper<T>::mutex;
template<typename T> QScopedPointer<T> SingletonHelper<T>::instance;

template<typename T>
T * SingletonHelper<T>::getInstance()
{
	if (instance.isNull())
	{
		mutex.lock();
		if (instance.isNull())
		{
			instance.reset(new T());
		}
		mutex.unlock();
	}

	return instance.data();
}

//���ֺ�
//==================================================================================================
#define SINGLETONHELPER(Class)						\
private:											\
	Class();										\
	~Class();										\
	friend class SingletonHelper<Class>;




