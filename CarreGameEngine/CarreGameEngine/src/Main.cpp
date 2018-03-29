#include "GL/glew.h"									// Include the GLEW library to manage OpenGL extensions
#include "..\headers\Main.h"							// Include our main header for the application


Model g_Model;										// Our class to handle initializing and drawing our model

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
	if ( !WindowManager || WindowManager->Initialize(ScreenWidth, ScreenHeight, "Carre Game Engine", false) != 0 )
	{
		// Quit the application if the window couldn't be created with an OpenGL context
		exit(-1);
	}

	// Tell OpenGL that we want a 3D viewport the same size as the window
	glViewport(0, 0, ScreenWidth, ScreenHeight);
	
	// This tells OpenGL that we want depth testing so it renders the order correctly
	glEnable(GL_DEPTH_TEST);

	// Below, instead of drawing triangles we draw 3D rectangles, or quads as they are called.
	// OpenGL has mostly evolved into just using triangles so quads aren't really supported in
	// the future, so to make a quad we need to build it from 2 triangles.

	// Below there is about 20 quads drawn upwards to form a tower type shape.  I slowly make
	// each quad smaller and it makes an interesting effect once it goes negative.

	// Initialize enough vertices to make 20 quads (6 verts * 20 quads = 120 vertices).
	// Notice that we converted our Vertex3 into a struct that uses vec3 and vec4's for
	// the xyz and rgba values.  This makes it easier to initialize and use going forward.
	// We set all vertices to position 0 and color 1 (white)
	Vertex3 vertices[6] = { vec3(0), vec4(1) };
	
	// Loop through 20 quads and build them from the ground to the ceiling.  The i is a float
	// so we can do division with i without having to cast every single one.  It takes 6
	// vertices to make a quad (2 triangles).  These will be lying flat instead of standing up.
	// The i/10 added or subtracted to the x and z positions gives the interesting effect of the
	// quads getting smaller as they go up and then going from smaller to larger again.
	
	// The first half of the quad (triangle 1)

	// The back left vertex -- colored yellow
	vertices[0].xyz = vec3(-1.0f, -1.0f, -1.0f);
	vertices[0].rgba = vec4(1, 1, 0, 1);

	// The back right vertex -- colored red
	vertices[1].xyz = vec3(1.0f, -1.0f, -1.0f);
	vertices[1].rgba = vec4(1, 0, 0, 1);

	// The front right vertex -- colored cyan
	vertices[2].xyz = vec3(1.0f, -1.0f, 1.0f);
	vertices[2].rgba = vec4(0, 1, 1, 1);

	// The second half of the quad (triangle 2)

	// The front right vertex -- colored cyan
	vertices[3].xyz = vec3(1.0f, -1.0f, 1.0f);
	vertices[3].rgba = vec4(0, 1, 1, 1);

	// The front left vertex -- colored blue
	vertices[4].xyz = vec3(-1.0f, -1.0f, 1.0f);
	vertices[4].rgba = vec4(0, 0, 1, 1);

	// The back left vertex -- colored yellow
	vertices[5].xyz = vec3(-1.0f, -1.0f, -1.0f);
	vertices[5].rgba = vec4(1, 1, 0, 1);
	
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	// Initialize the model with the vertex array and give the vertex length of 120
	g_Model.Initialize(vertices, 6, source.VertexSource, source.FragmentSource);
		
	// Create the projection matrix from our camera and make the near field closer and the far field farther.
	// This makes it so our tower doesn't get cut off and also doesn't cull geometry right near the camera.
	//									 FOV		    Aspect Ratio			   Near / Far Planes
	Camera->SetPerspective(glm::radians(60.0f), ScreenWidth / (float)ScreenHeight, 0.01f, 1000);

	//					  Position	  Yaw	 Pitch
	Camera->PositionCamera(0, 0, 6,		0,		0);

	// We now pass in the camera to have access to the projection and view matrices
	g_Model.SetCamera(Camera);
	
	// Set the position of the model to be at the origin
	g_Model.SetPosition(vec3(0, 0, 0));


	/**************************************************************************/
	// Create static rigid body (floor)
	physicsWorld.CreateStaticRigidBody();
	
	// Create dynamic rigid bodies
	physicsWorld.CreateDynamicRigidBody(btVector3(15.0, 70.0, 13.0));
	physicsWorld.CreateDynamicRigidBody(btVector3(15.0, 15.0, 15.0));

	// Add body positions to array for drawing (initial position)
	collisionBodyPos.push_back(btVector3(0.0, 1.0, 0.0));
	collisionBodyPos.push_back(btVector3(15.0, 70.0, 13.0));
	collisionBodyPos.push_back(btVector3(15.0, 15.0, 15.0));


	quad = gluNewQuadric();
	/**************************************************************************/
}


// This is our game loop where all the magic happens every frame
void GLApplication::GameLoop()
{
	// Loop until the user hits the Escape key or closes the window
	while ( WindowManager->ProcessInput(true) )
	{
		// Use our Singleton to calculate our framerate every frame, passing true to set FPS in titlebar
		TimeManager::Instance().CalculateFrameRate(true);

		// This clears the screen every frame to black (color can be changed with glClearColor)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		// Render the model
		//g_Model.Render();

		// Render a floor on x/z axis (10 x 10)
		for (int x = 0; x < 15; x++)
		{
			for (int z = 0; z < 15; z++)
			{
				g_Model.SetPosition(vec3(x * 2, 0, z * 2));
				g_Model.Render();
			}
		}

		/**************************************************************************/
		// Update physicsWorld
		physicsWorld.Simulate(collisionBodyPos);
		
		// Draw shapes for testing (just planes atm, didn't know how to make spheres using current setup)
		for (int i = 0; i < 3; i++)
		{
			//int a = collisionBodyPos[i].x;
			vec3 temp = vec3(collisionBodyPos[i].x(), collisionBodyPos[i].y(), collisionBodyPos[i].z());

			/*glPushMatrix();
			glColor3f(0.0, 0.0, 0.5);
			glTranslatef(collisionBodyPos[i].x(), collisionBodyPos[i].y(), collisionBodyPos[i].z());
			gluSphere(quad, 5, 100, 100);
			glPopMatrix();*/

			g_Model.SetPosition(vec3(temp.x, temp.y, temp.z));
			g_Model.Render();
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
	g_Model.Destroy();

	// If we have a window manager still allocated then destroy and delete it
	if ( WindowManager )
	{
		WindowManager->Destroy();

		delete WindowManager;
		WindowManager = nullptr;
	}
	
	// If we have the camera still, delete it
	if ( Camera )
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