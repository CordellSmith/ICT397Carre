#include "GL/glew.h"									// Include the GLEW library to manage OpenGL extensions

#include "..\headers\ObjLoader.h"
#include "..\headers\GameWorld.h"							// Include our main header for the application

Model colourPanel;										// Our class to handle initializing and drawing our model
Model cubeModel;

objl::Loader Loader;

Vertex3 panel[6] = { vec3(0), vec4(1) };
Vertex3 cube[108] = { vec3(0), vec4(1) };

void prepareCube()
{
	float cube_vertex_array[] = {
		-1.0f,-1.0f,-1.0f, // triangle 1 : begin
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f,-1.0f, // triangle 2 : begin
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f, // triangle 2 : end
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

	float red = 0.0f, blue = 0.5f, green = 1.0f;
	int j = 0;
	int x, y, z;
	for (int i = 0; i < 36; i++)
	{
		x = cube_vertex_array[j];
		y = cube_vertex_array[j + 1];
		z = cube_vertex_array[j + 2];

		// Vertices
		cube[i].xyz = vec3(x, y, z);

		// Colour (random fx)
		//if (i % 3 == 0)
		//{
		//	red = 1.0f;
		//	blue = 0.0f;
		//	green = 0.0f;
		//}
		//else if (i % 5 == 0)
		//{
		//	red = 0.0f;
		//	blue = 1.0f;
		//	green = 0.0f;
		//}
		//else if (i % 7 == 0)
		//{
		//	red = 0.0f;
		//	blue = 0.0f;
		//	green = 1.0f;
		//}
		cube[i].rgba = vec4(red, green, blue, 1.0);

		j += 3;
		// Used for testing
		std::cout << "X:" << cube[i].xyz[0] << " Y:" << cube[i].xyz[1] << " Z:" << cube[i].xyz[2] << std::endl;
	}
}

void preparePanel()
{
	// The first half of the quad (triangle 1)
	// The back left vertex -- colored yellow
	panel[0].xyz = vec3(-1.0f, -1.0f, -1.0f);
	panel[0].rgba = vec4(1, 1, 0, 1);

	// The back right vertex -- colored red
	panel[1].xyz = vec3(1.0f, -1.0f, -1.0f);
	panel[1].rgba = vec4(1, 0, 0, 1);

	// The front right vertex -- colored cyan
	panel[2].xyz = vec3(1.0f, -1.0f, 1.0f);
	panel[2].rgba = vec4(0, 1, 1, 1);

	// The second half of the quad (triangle 2)

	// The front right vertex -- colored cyan
	panel[3].xyz = vec3(1.0f, -1.0f, 1.0f);
	panel[3].rgba = vec4(0, 1, 1, 1);

	// The front left vertex -- colored blue
	panel[4].xyz = vec3(-1.0f, -1.0f, 1.0f);
	panel[4].rgba = vec4(0, 0, 1, 1);

	// The back left vertex -- colored yellow
	panel[5].xyz = vec3(-1.0f, -1.0f, -1.0f);
	panel[5].rgba = vec4(1, 1, 0, 1);
}

/***********************************************/
//Model sphereModel;
/***********************************************/

// This is our own main() function which abstracts the required main() function to run this application.
int GLApplication::GLMain()
{
	// This calls our Initialize() function below which creates the window and triangle
	Initialize();

	// This is our main game loop which will run until we close the window or hit Escape.
	GameLoop();

	// Once we hit Escape this will clean up the application's resources.
	Destroy();

	// Return 0 for success
	return 0;
}


// This function initializes the window, the shaders and the triangle vertex data.
void GLApplication::Initialize()
{
	// Make sure the window manager is initialized prior to calling this and creates the OpenGL context
	if (!WindowManager || WindowManager->Initialize(ScreenWidth, ScreenHeight, "Carre Game Engine", false) != 0)
	{
		// Quit the application if the window couldn't be created with an OpenGL context
		exit(-1);
	}

	// Tell OpenGL that we want a 3D viewport the same size as the window
	glViewport(0, 0, ScreenWidth, ScreenHeight);

	// This tells OpenGL that we want depth testing so it renders the order correctly
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	///////////////////////////////////////////////////////////////////
	/* OBJ Loader Testing */
	bool loadout = Loader.LoadFile("res/objects/box_stack.obj");
	if (loadout)
		std::cout << "SUCCESS LOADING OBJ FILE" << std::endl;
	else
		std::cout << "FAILED TO LOAD OBJ FILE" << std::endl;
	///////////////////////////////////////////////////////////////////

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	ShaderProgramSource cubeSource = ParseShader("res/shaders/Cube.shader");

	preparePanel();
	prepareCube();

	// Initialize the model with the vertex array and give the vertex length of 120
	colourPanel.Initialize(panel, 6, source.VertexSource, source.FragmentSource);
	cubeModel.Initialize(cube, 36, cubeSource.VertexSource, cubeSource.FragmentSource);

	// Create the projection matrix from our camera and make the near field closer and the far field farther.
	// This makes it so our tower doesn't get cut off and also doesn't cull geometry right near the camera.
	//									 FOV		    Aspect Ratio			   Near / Far Planes
	Camera->SetPerspective(glm::radians(60.0f), ScreenWidth / (float)ScreenHeight, 0.01f, 100);

	//					  Position	  Yaw	 Pitch
	Camera->PositionCamera(0, 0, 6, 0, 0);

	// We now pass in the camera to have access to the projection and view matrices
	colourPanel.SetCamera(Camera);
	cubeModel.SetCamera(Camera);

	/* Doesnt really need to be called because we are dynamically calling them in the loop */
	// Set the position of the model to be at the origin
	//colourPanel.SetPosition(vec3(0, 0, 0));
	//cubeModel.SetPosition(vec3(2, 5, 2));

	// Physics Testing
	// Create static rigid body (floor)
	physicsWorld.CreateStaticRigidBody();
	// Create dynamic rigid bodies
	physicsWorld.CreateDynamicRigidBody(btVector3(15.0, 30.0, 15.0));
	physicsWorld.CreateDynamicRigidBody(btVector3(15.0, 0.0, 15.0));

	// Add body positions to array for drawing (initial position)
	collisionBodyPos.push_back(btVector3(0.0, 0.0, 0.0));
	// Position of first object
	collisionBodyPos.push_back(btVector3(15.0, 30.0, 13.0));
	// Position of second object
	collisionBodyPos.push_back(btVector3(15.0, 0.0, 15.0));

	// Resource Factory Testing
	factory.CreateResource(RESOURCE_TEXTURE);
}


// This is our game loop where all the magic happens every frame
void GLApplication::GameLoop()
{
	// Loop until the user hits the Escape key or closes the window
	while (WindowManager->ProcessInput(true))
	{
		// Use our Singleton to calculate our framerate every frame, passing true to set FPS in titlebar
		TimeManager::Instance().CalculateFrameRate(true);

		// This clears the screen every frame to black (color can be changed with glClearColor)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render a floor on x/z axis (15 x 15)
		for (int x = 0; x < 15; x++)
		{
			for (int z = 0; z < 15; z++)
			{
				colourPanel.SetPosition(vec3(x * 2, 0, z * 2));
				colourPanel.Render();
			}
		}

		/**************************************************************************/
		// Update physicsWorld
		physicsWorld.Simulate(collisionBodyPos);

		// Draw shapes for testing (just planes atm, didn't know how to make spheres using current setup)
		vec3 temp = vec3(collisionBodyPos[0].x(), collisionBodyPos[0].y(), collisionBodyPos[0].z());
		colourPanel.SetPosition(vec3(temp.x, temp.y, temp.z));

		for (int i = 1; i < 3; i++)
		{
			//int a = collisionBodyPos[i].x;
			vec3 temp = vec3(collisionBodyPos[i].x(), collisionBodyPos[i].y(), collisionBodyPos[i].z());

			/*glPushMatrix();
			glColor3f(0.0, 0.0, 0.5);
			glTranslatef(collisionBodyPos[i].x(), collisionBodyPos[i].y(), collisionBodyPos[i].z());
			gluSphere(quad, 5, 100, 100);
			glPopMatrix();*/

			cubeModel.SetPosition(vec3(temp.x, temp.y, temp.z));
			cubeModel.Render();
		}
		/**************************************************************************/

		// Swap the buffers to display the final rendered image on screen
		WindowManager->SwapTheBuffers();
	}
}


// This can be used to free all of our resources in the application.
void GLApplication::Destroy()
{
	// Free the vertex buffers and array objects
	//colourPanel.Destroy();

	// If we have a window manager still allocated then destroy and delete it
	if (WindowManager)
	{
		WindowManager->Destroy();

		delete WindowManager;
		WindowManager = nullptr;
	}

	// If we have the camera still, delete it
	if (Camera)
	{
		delete Camera;
		Camera = nullptr;
	}
}

// Handles reading in the Basic.shader file containing vertex and fragment shader information and splits it into two strings then returns a struct ShaderProgramSource containing these two strings
ShaderProgramSource GLApplication::ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2]; // stack allocated array that will store vertex and fragment strings
	ShaderType type = ShaderType::NONE; // set to none (-1) by default
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return{ ss[0].str(), ss[1].str() };
}

// Compiles the shader taking its type either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER and a string containing the shader information
unsigned int GLApplication::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);

		return 0;
	}

	return id;
}

// Creates the shader taking two strings containing vertex shader information and fragment shader information
unsigned int GLApplication::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}