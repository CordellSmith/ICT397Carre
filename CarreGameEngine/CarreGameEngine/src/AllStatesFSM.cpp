/*
* Implementation of AllStatesFSM.h file
* Author - Jack Matters
*/

// Includes
#include <iostream>
#include "../headers/AllStatesFSM.h"

/*****************************************Class Separator******************************************/

void IdleState::Enter(ComputerAI* compAI)
{
	std::cout << "Entering 'Enter' state!" << std::endl;

	isMoving = false;
	m_waypoints = compAI->MakeWaypoints();
	currTargetPos = m_waypoints[0];
}

void IdleState::Execute(ComputerAI* compAI)
{
	// If no velocity, set to walking and pick a waypoint
	Vector2 tempVel = compAI->GetVelocity();
	if (tempVel.x == 0 && tempVel.z == 0)
	{
		compAI->SetVelocity(Vector2(5, 0));

		//srand(time(NULL));
		int pos = rand() % m_waypoints.size();
		currTargetPos = m_waypoints[pos];

		std::cout << currTargetPos << std::endl;
	}

	compAI->MoveTo(compAI, currTargetPos);
	
	//std::cout << compAI->GetVelocity() << std::endl;
	//std::cout << compAI->GetPosition() << std::endl;
}

void IdleState::Exit(ComputerAI* compAI)
{
	std::cout << "Entering 'Exit' state!" << std::endl;
}

/*****************************************Class Separator******************************************/

void StartState::Execute(ComputerAI* compAI)
{
	compAI->GetFSM()->ChangeState(&m_idleState::GetInstance());
}

/*****************************************Class Separator******************************************/