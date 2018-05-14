/**
* @class ScriptManager
* @brief Script manager class that handles the loading of all scripts
* @author Jack Matters
*
* @date 14/07/2018
* @version 1.0	Initial start. Created the layout of the class, using a singleton.
*
* @date 14/07/2018
* @version 1.1	
*/

#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

// Includes
#include "lua.hpp"
//#pragma comment(lib,"lua5.1.lib")

class ScriptManager
{
	public:
		
			/**
			* @brief Singleton instance
			*
			* This function returns a singleton instance of the script manager class so that there is only
			* one instance of a script manager.
			*
			* @return static TextureManager&
			*/
		static ScriptManager& Instance()
		{
			static ScriptManager instance;

			return instance;
		}

			/**
			* @brief Load all lua scripts
			*
			* Public function that calls a private function for each type of script to load
			*
			* @return void
			*/
		void LoadAllLuaScripts();

		

	private:

			/**
			* @brief Default constructor
			*
			* This is the default constructor
			*
			* @return null
			*/
		ScriptManager();

			/**
			* @brief De-constructor
			*
			* This is the deconstructor
			*/
		~ScriptManager();

			/**
			* @brief Load cam initilization
			*
			* Loads all camera and window initialisation variables in
			*
			* @return bool - True if load success, else false
			*/
		bool LoadCamInitLua();

};

#endif