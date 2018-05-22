#pragma once

#include <vector>
#include <GL\glew.h>

#include "..\Common\MyMath.h"
#include "..\AssetFactory\Model.h"
//#include "IRenderer.h" // Will make this class use IRenderer later

class OpenGl
{
public:
	OpenGl() { }
	~OpenGl() { }

	void Prepare(Model* model, std::string vertShader, std::string fragShader);
	void Render(Model* model);

protected:

};