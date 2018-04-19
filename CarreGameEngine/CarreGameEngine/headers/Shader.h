		/**
		* @class Shader
		* @brief
		* This is our very basic shader class that we will use to load and turn on/off our shaders
		*
		* @author
		* @version 01
		* @date 29/03/2018 CS
		*
		*/

#pragma once

#include <string>										// Used for our STL string objects
#include <fstream>										// Used for our ifstream object to load text files
#include "GL\glew.h"									// Used for the OpenGL types like GLuint, GLfloat, etc.

class Shader
{
public:
		/**
		* @brief Default constructor
		* Create an empty constructor
		*
		* @return null
		*/
	Shader()	{ }
		/**
		* @brief Destructor
		* Have the deconstructor release our memory.
		*
		* @return null
		*/
	~Shader()	{ Destroy(); }

		/**
		* @brief 
		* This loads our text file for each 
		* shader and returns it in a string
		* @param std::string strFile
		*
		* @return std::string
		*/
	std::string LoadShaderFile(std::string strFile);

		/**
		* @brief
		* This loads a vertex and fragment shader 
		* from a text file (relative or full path)
		* @param std::string strVertexFile
		* @param std::string strFragmentFile 
		*
		* @return void
		*/
	void Initialize(std::string strVertexFile, std::string strFragmentFile );
	
		/**
		* @brief
		* This returns an ID for a variable in our shader, 
		* to be used with a Set*() function
		* @param std::string strVariable
		*
		* @return GLint
		*/
	GLint GetVariable(std::string strVariable);

	// Below are functions to set an integer, a set of floats or a matrix (float[16])
	void SetInt(GLint id, int newValue)											{ glUniform1i(id, newValue);		}
	void SetFloat(GLint id, GLfloat newValue)									{ glUniform1f(id, newValue);		}
	void SetFloat2(GLint id, GLfloat v0, GLfloat v1)							{ glUniform2f(id, v0, v1);			}
	void SetFloat3(GLint id, GLfloat v0, GLfloat v1, GLfloat v2)				{ glUniform3f(id, v0, v1, v2);		}
	void SetFloat4(GLint id, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)	{ glUniform4f(id, v0, v1, v2, v3);	}
	
		/**
		* @brief
		* This allows us to pass in 4x4 matrix (float array of 16) 
		* to a shader by the Id received from GetVariable()
		* @param GLint id, GLsizei count
		* @param GLboolean transpose 
		* @param const GLfloat *value
		*
		* @return void
		*/
	void SetMatrix4(GLint id, GLsizei count, GLboolean transpose, const GLfloat *value)
	{ 
		glUniformMatrix4fv(id, count, transpose, value);
	}

		/**
		* @brief
		* Turn on our shader, 
		* which uses the OpenGL glUseProgram() function
		*
		* @return void
		*/
	void TurnOn()		{	glUseProgram(ShaderProgramId);  }
		/**
		* @brief
		* Turn off our shader,
		* which uses the OpenGL glUseProgram() function
		*
		* @return void
		*/
	void TurnOff()		{	glUseProgram(0);				}
	
	// This releases our memory for our shader
	void Destroy();

private:
	// This Id stores our vertex shader information
	GLuint VertexShaderId;

	// This Id stores our fragment shader information
	GLuint FragmentShaderId;

	// This Id stores our program information which encompasses our shaders
	GLuint ShaderProgramId;
};
