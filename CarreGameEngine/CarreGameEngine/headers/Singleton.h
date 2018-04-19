#pragma once

	/**
	* @class Singleton
	* @brief Template singleton class 
	*
	* This singleton class is used to create single instances of a given type.
	*
	* @author Cordell Smith
	* @version 01
	* @date 29/03/2018 CS
	*
	*/
template<class T>
class Singleton
{
public:
	/**
	* @brief Gets the instance of singleton
	*
	* This function returns the singleton instance of the class type.
	*
	* @return static Static T*
	*/
	Static T* GetInstance() {
		static T instance;
		
		return &instance;
	}

private:
		/**
		* @brief Default constructor
		*
		* This is the default constuctor that is empyty and private so it cannot be accessed.
		*
		* @return null
		*/
	Singleton() { }

		/**
		* @brief Destructor
		*
		* This is the destructor that is empyty and private so it cannot be accessed.
		*
		* @return null
		*/
	~Singleton() { }

		/**
		* @brief Copy constructor
		*
		* This is the copy constuctor and is private so it cannot be accessed.
		*
		* @return null
		*/
	Singleton(const Singleton &);

		/**
		* @brief Overloaded assignment operator
		*
		* This is the overloaded assignment operator.
		*
		* @param const Singleton&
		* @return Singleton&
		*/
	Singleton &operator=(const Singleton&);
};