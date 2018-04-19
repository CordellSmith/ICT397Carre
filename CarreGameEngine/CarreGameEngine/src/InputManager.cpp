#include "..\headers\InputManager.h"
#include "..\headers\TimeManager.h"


// This takes in a code and depending on the value moves the camera
void InputManager::KeyPressed(InputCodes code)
{
	// If there isn't a camera specified, just return
	if (m_camera == nullptr)
		return;

	// Depending on the code, move the camera back or forward fixed by a speed and time slice
	// to make sure the movement is the same on anyone's computer regardless of the speed.
	switch ( code )
	{
		// If the user moves up, move the camera up along the view vector
		case Up: case W: case w:
			m_camera->MoveCamera(m_camera->GetSpeed() * TimeManager::Instance().DeltaTime);
			break;
		// If the user moved down, move the camera down along the view vector
		case Down: case S: case s:
			m_camera->MoveCamera(-1 * m_camera->GetSpeed() * TimeManager::Instance().DeltaTime);
			break;
		// Used to toggle wireframe of vertices
		case q: case Q:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case 101: case E:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
	}
}

// This sends in an updated mouse x and y position that should be a delta from (0, 0)
void InputManager::MouseMoved(float mouseX, float mouseY)
{
	// Return if we don't have a valid camera assigned
	if (m_camera == nullptr)
		return;

	// Have our manager send the mouse x and y deltas to our camera to process the rotations
	m_camera->SetViewByMouse(mouseX, mouseY);
}