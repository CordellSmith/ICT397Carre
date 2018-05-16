#include "..\headers\InputManager.h"
#include "..\headers\TimeManager.h"

void InputManager::KeyPressed(InputCodes code)
{
	// Validate camera
	if (m_camera == nullptr)
		return;

	switch (code)
	{
		// Move forwards
		case Up: case W: case w:
			//m_camera->MoveCamera((float)(m_camera->GetSpeed() * TimeManager::Instance().DeltaTime));
			m_player->MoveForward((float)(m_player->GetMoveSpeed() * TimeManager::Instance().DeltaTime));
			break;
		// Move backwards
		case Down: case S: case s:
			//m_camera->MoveCamera((float)(-1 * m_camera->GetSpeed() * TimeManager::Instance().DeltaTime));
			m_player->MoveBackward((float)(m_player->GetMoveSpeed() * TimeManager::Instance().DeltaTime));
			break;
		case Left: case A: case a:
			//m_camera->MoveCamera((float)(-1 * m_camera->GetSpeed() * TimeManager::Instance().DeltaTime));
			m_player->TurnAntiClock((float)(m_player->GetMoveSpeed() * TimeManager::Instance().DeltaTime));
			break;
		case Right: case D: case d:
			//m_camera->MoveCamera((float)(-1 * m_camera->GetSpeed() * TimeManager::Instance().DeltaTime));
			m_player->TurnClock((float)(m_player->GetMoveSpeed() * TimeManager::Instance().DeltaTime));
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

void InputManager::MouseMoved(float mouseX, float mouseY)
{
	if (m_camera == nullptr)
		return;

	m_camera->SetViewByMouse(mouseX, mouseY);
}

void InputManager::MousePressed(InputCodes code, float mouseX, float mouseY)
{
	if (m_camera == nullptr)
		return;

	switch (code)
	{
	case MouseButtonLeft:
		m_camera->ChangeAngleAroundPlayer(mouseX);
		break;
	case MouseButtonRight:
		m_camera->ChangePitch(mouseY);
		break;
	default:
		break;
	}
}

void InputManager::WheelScrolled(double yoffset)
{
	if (m_camera == nullptr)
		return;

	//std::cout << fov << std::endl;
	m_camera->Zoom(yoffset);
}