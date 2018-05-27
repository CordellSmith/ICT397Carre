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
	this->m_currPos = Vector2(0, 0);
	this->m_currVel = Vector2(1, 0);
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

// Return if dead or not
bool ComputerAI::IsDead()
{
	return m_isDead == true;
}

// Update the FSM
void ComputerAI::Update()
{
	m_computerAIFSM->Update();
}

// Set current velocity
void ComputerAI::SetVelocity(Vector2 vel)
{
	this->m_currVel = vel;
}

// Get current velocity
Vector2 ComputerAI::GetVelocity()
{
	return m_currVel;
}

// Set current position
void ComputerAI::SetPosition(Vector2 pos)
{
	this->m_currPos = pos;
}

// Return current position
Vector2 ComputerAI::GetPosition()
{
	return m_currPos;
}

// Move to a location
bool ComputerAI::MoveTo(ComputerAI* compAI)
{
	Vector2 targetPos(100, 100);
	Vector2 currVel = compAI->GetVelocity();
	Vector2 currPos = compAI->GetPosition();

	// Calcute heading from this position to target position
	Vector2 toTarget = targetPos - currPos;
	toTarget = toTarget.Normalized();
	if(toTarget.x == 0 && toTarget.z == 0)
		return true;

	// Calculate new velocity and new position
	currVel = toTarget * currVel.Length();
	compAI->SetVelocity(currVel);
	Vector2 displacement = currVel * 0.05;
	Vector2 newPos = currPos + displacement;

	// Calculate real target position
	Vector2 realTargetPos = targetPos - (toTarget * 0.5);

	// Calculate the direction from newPos to realTargetPos
	Vector2 toRealTarget = realTargetPos - newPos;
	toRealTarget.Normalized();
	if (toRealTarget.x == 0 && toRealTarget.z == 0)
	{
		currPos = realTargetPos;
		compAI->SetPosition(currPos);
		return true;
	}
	
	// Check to see whether newPos has passed the realTargetPos
	float dp = toRealTarget.Dot(toRealTarget, toTarget);
	if (dp < 0.0)
	{
		currPos = realTargetPos;
		compAI->SetPosition(currPos);
		return true;
	}

	// newPos has not yet passed realTargetPos
	currPos = newPos;
	compAI->SetPosition(currPos);
	return false;
}

