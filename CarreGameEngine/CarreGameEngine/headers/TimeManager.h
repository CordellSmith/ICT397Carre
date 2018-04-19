/**
* @class TimeManager
* @brief This is our TimeManager singleton class to calculate 
* our frame rate and give a time slice. We use the Singleton 
* design pattern so that we don't need to pass in the time slice everywhere.
*
* @author
* @version 01
* @date 29/03/2018 CS
*
*/
#pragma once

#include <chrono>													// Used for the C++ 11 time functionality
#include <thread>													// used for the C++ 11 sleep functionality


// 
class TimeManager
{
public:

		/**
		* @brief The function to get the instance of the manager, 
		* or initialize and return the instance.
		* By creating a static variable of the instance we ensure 
		* this only gets created once. This is also thread-safe in 
		* C++ 11 according to the specifications.
		*
		* @return instance
		*/
	static TimeManager& Instance()
	{
		static TimeManager instance;
		
		return instance;
	}

		/**
		* @brief This calculates our current scene's 
		* frames per second and displays it in the console
		* @param bool writeToConsole
		*
		* @return double
		*/
	double CalculateFrameRate(bool writeToConsole);

	/**
	* @brief This returns the current time in seconds 
	* (since 1/1/1970, call "epoch")
	*
	* @return double
	*/
	double TimeManager::GetTime();

	/**
	* @brief This pauses the current thread for 
	* an amount of time in milliseconds)
	* @param int milliseconds
	*
	* @return void
	*/
	void Sleep(int milliseconds);

	/**
	* @brief This is the time slice that stores the 
	* total time in seconds that has elapsed since the last frame
	*
	* @return double
	*/
	double DeltaTime = 0;

	/**
	* @brief This is the current time in seconds
	*
	* @return double
	*/
	double CurrentTime = 0;

private:
		/**
		* @brief Default constructor
		* Create an empty constructor
		*
		* @return null
		*/
	TimeManager(){};								// Private so that it can  not be called
		/**
		* @brief Destructor
		*
		* @return null
		*/
	TimeManager(TimeManager const&);				// copy constructor is private
	TimeManager& operator=(TimeManager const&);		// assignment operator is private
};
