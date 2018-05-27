/*
* Implementation of ComputerAI.h file
* Author - Jack Matters
*/

// Includes
#include "../headers/ComputerAI.h"
#include "../headers/AllStatesFSM.h"

// Default constructor
ComputerAI::ComputerAI()
{
	this->m_computerAIFSM = new StateMachine<ComputerAI>(this);

	// Possible change
	this->m_computerAIFSM->SetCurrState(&m_idleState::GetInstance());
	this->m_computerAIFSM->SetGlobalState(&m_globalState::GetInstance());

	// Read from script
	this->m_currPos = Vector3(0, 0, 0);
	this->m_health = 100;
	this->m_maxHealth = 100;
	this->m_isDead = false;
}

// De-constructor
ComputerAI::~ComputerAI()
{
	delete m_computerAIFSM;
}

// Add health
void ComputerAI::AddHealth(int health)
{
	this->m_health += health;

	// Don't go over max health
	if (this->m_health > this->m_maxHealth)
		this->m_health = this->m_maxHealth;
}

// Decrease health
void ComputerAI::DecreaseHealth(int health)
{
	this->m_health -= health;

	// Check to see if dead
	if (this->m_health < 0)
	{
		this->m_health = 0;
		m_isDead = true;
	}
}

// Return health
int ComputerAI::GetHealth()
{
	return this->m_health;
}

// Move to a location
bool ComputerAI::MoveTo(Vector3 &currPos, Vector3 &targetPos, Vector3 &currVel, double timeElapsed, double offset)
{
	// Calcute heading from this position to target position
	Vector3 toTarget = targetPos - currPos;
	toTarget.Normalized();
	if(toTarget.Length() == 0)
		return true;

	// Calculate new velocity and new position
	currVel = toTarget * currVel.Length();
	Vector3 displacement = currVel * timeElapsed;
	Vector3 newPos = currPos + displacement;

	// Calculate real target position
	Vector3 realTargetPos = targetPos - (toTarget * offset);

	// Calculate the direction from newPos to realTargetPos
	Vector3 toRealTarget = realTargetPos - newPos;
	toRealTarget.Normalized();
	if (toRealTarget.Length() == 0)
	{
		currPos = realTargetPos;
		return true;
	}

	// Check to see whether newPos has passed the realTargetPos
	float dp = toRealTarget.Dot(toRealTarget, toTarget);
	if (dp < 0.0)
	{
		currPos = realTargetPos;
		return true;
	}

	// newPos has not yet passed realTargetPos
	currPos = newPos;
	return false;
}