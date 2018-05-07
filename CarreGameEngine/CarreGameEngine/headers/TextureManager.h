/**
* @class TextureManager
* @brief Texture manager class that handles the loading, storing, and managing of all textures
* @author Jack Matters
*
* @date 03/07/2018
* @version 1.0	Initial start. Created the layout of the class, using a singleton and the SOIL API.
*/

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

// Includes
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

class TextureManager
{
	public:

			/**
			* @brief Singleton instance
			*
			* This function returns a singleton instance of the texture manager class so that there is only
			* one instance of a texture manager.
			*
			* @return static TextureManager&
			*/
		static TextureManager& Instance()
		{
			static TextureManager instance;

			return instance;
		}

			/**
			* @brief Default constructor
			*
			* This is the default constructor
			*
			* @return null
			*/
		TextureManager();

			/**
			* @brief De-constructor
			*
			* This is the deconstructor 
			*/
		~TextureManager();

			/**
			* @brief Set texture to be active
			*
			* Sets the given texture to be able, and binds it
			*
			* @param texID - The texture to set active
			*
			* @return bool - True
			*/
		bool SetActiveTexture(unsigned int texID);

			/**
			* @brief Load a texture
			*
			* Load a texture from file using the SOIL API
			*
			* @param filename - Name of file to load
			*
			* @return int - texID (given ID of loaded texture)
			*/
		int LoadTexture(std::string filename);

			/**
			* @brief Add a texture to the map
			*
			* Adds a texture that is loaded into the map for storing and later access
			*
			* @param filename - Name of file loaded
			* @param texID - ID of texture
			*
			* @return void
			*/
		void AddTextureToMap(std::string filename, int texID);

			/**
			* @brief Release all textures
			*
			* Releases all textures from memory (called in de-constructor
			*
			* @return void
			*/
		void ReleaseAllTextures();

	private:

			/**
			* @brief
			*
			* An unordered map containing all textures for easy lookup
			*/
		std::unordered_map<std::string, unsigned int> m_textureMap;

		/// Number of textures
		int m_numTextures;
};

#endif