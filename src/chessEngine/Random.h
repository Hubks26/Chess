#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <chrono>

#include "Motor.h"

class Random : public Motor
{
	public :
		Random(Board&);

        virtual Move getMove();

	private :
		Board& m_board;
};

#endif //RANDOM_H
