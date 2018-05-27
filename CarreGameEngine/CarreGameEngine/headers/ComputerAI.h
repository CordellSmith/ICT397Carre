/**
* @class State
* @brief ComputerAI class that handles all AI behaviour
* @author Jack Matters
*
* @date 27/05/2018
* @version 1.0	Initial start. Created the layout of the class.
*/

#ifndef COMPUTERAI_H
#define COMPUTERAI_H

// Includes
#include "State.h"
#include <vector>
#include "StateMachine.h"


class ComputerAI
{
	public:

			/**
			* @brief Default constructor
			*
			* This is the default constructor
			*/
		ComputerAI();

			/**
			* @brief De-constructor
			*
			* This is the de-constructor
			*/
		~ComputerAI();

			/**
			* @brief Increase health
			*
			* Increase the total value of health, up to maximum
			*
			* @param health - Amount to increase
			*
			* @return void
			*/
		void AddHealth(int health);

			/**
			* @brief Decrease health
			*
			* Decrease the total value of health
			*
			* @param health - Amount to decrease
			*
			* @return void
			*/
		void DecreaseHealth(int health);

			/**
			* @brief Returns health
			*
			* Return the current health point value
			*
			* @return int - m_health
			*/
		int GetHealth();
		
		bool MoveTo(Vector3 &currPos, Vector3 &targetPos, Vector3 &currVel, double timeElapsed, double offset);

	private:

		/// Return an instance of the current FSM state
		StateMachine<ComputerAI>* m_computerAIFSM;

		/// Current XZ position
		Vector3 m_currPos;

		/// Current health points
		int m_health;

		/// Maximum health points
		int m_maxHealth;

		///
		bool m_isDead;
};
#endif
