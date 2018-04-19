		/**
		* @class Singleton
		* @brief 
		*
		* @author
		* @version 01
		* @date 29/03/2018 CS
		*
		*/

#pragma once

template<class T>
class Singleton
{
public:
		/**
		* @brief Getting for the current instance
		* @param static T instance
		*
		* @return &instance
		*/
	Static T* GetInstance() {
		static T instance;
		
		return &instance;
	}

private:
		/**
		* @brief Default constructor
		*
		* @return null
		*/
	Singleton();
		/**
		* @brief Destructor
		*
		* @return null
		*/
	~Singleton();

	// Hidden
	Singleton(const Singleton &);
	Singleton &operator=(const Singleton &);
};