/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   SingletonT.h
				created:    5/29/2012
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#pragma once

namespace Tank
{
	/**
		Singleton class template 
        After inheriting need to call SINGLETON_DEFINE (class) macro in the realization of singleton class
	*/
	template<typename T>
	class SingletonT
	{
	public:
		virtual ~SingletonT() { }

		static T* getSingletonPtr()
		{
			static T instance;
			return &instance;
		}

		static T& getSingleton() { return *getSingletonPtr(); }

	protected:
		SingletonT(){}
		SingletonT(const SingletonT&);
		SingletonT& operator=(const SingletonT&);
	};


	#define SINGLETON_DEFINE(classname) friend class SingletonT<classname>;
}
