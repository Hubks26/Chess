#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <chrono>

#include "Motor.h"

class Random : public Motor
{
	public :
		Random();

        virtual Move getMove(Board& board);
};

#endif //RANDOM_H
