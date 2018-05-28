/**
* @class AllStatesFSM
* @brief Contains all states (classes) related to the FSM
* @author Jack Matters
*
* @date 27/05/2018
* @version 1.0	Initial start. Created the layout of the class.
*/

#ifndef ALLSTATESFSM_H
#define ALLSTATESFSM_H

// Includes
#include "Singleton.h"
#include "State.h"
#include "ComputerAI.h"
#include "TimeManager.h"

/*****************************************Class Separator******************************************/

//class ComputerAI;

/*****************************************Class Separator******************************************/

class IdleState:public State<ComputerAI>
{
public:
	void Enter(ComputerAI* compAI);
	void Execute(ComputerAI* compAI);
	void Exit(ComputerAI* compAI);

	bool isMoving;
	std::vector<Vector2> m_waypoints;
	Vector2 currTargetPos;
};

/*****************************************Class Separator******************************************/

class GlobalState :public State<ComputerAI>
{
public:
	void Enter(ComputerAI* compAI) { std::cout << "Entering Global state!" << std::endl; };
	void Execute(ComputerAI* compAI) {  };
	void Exit(ComputerAI* compAI) {};
};

/*****************************************Class Separator******************************************/

class StartState :public State<ComputerAI>
{
public:
	void Enter(ComputerAI* compAI) {};
	void Execute(ComputerAI* compAI);
	void Exit(ComputerAI* compAI) {};
};

/*****************************************Class Separator******************************************/


typedef Singleton<IdleState> m_idleState;
typedef Singleton<GlobalState> m_globalState;
typedef Singleton<StartState> m_startState;

#endif