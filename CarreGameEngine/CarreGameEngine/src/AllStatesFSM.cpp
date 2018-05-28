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
}

void IdleState::Execute(ComputerAI* compAI)
{
	//std::cout << "Entering 'Execute' state!" << std::endl;

	// If no velocity, set to walking
	Vector2 tempVel = compAI->GetVelocity();
	if (tempVel.x == 0 && tempVel.z == 0)
		compAI->SetVelocity(Vector2(5, 0));

	compAI->MoveTo(compAI);
	
	//std::cout << compAI->GetVelocity() << std::endl;
	//std::cout << compAI->GetPosition() << std::endl;
}

void IdleState::Exit(ComputerAI* compAI)
{
	std::cout << "Entering 'Exit' state!" << std::endl;
}

/*****************************************Class Separator******************************************/