#ifndef ALPHA_BETA_H
#define ALPHA_BETA_H

#include <random>
#include <chrono>

#include "Motor.h"

class AlphaBeta : public Motor
{
	public :
		AlphaBeta();

        int eval(Board&);
        virtual Move getMove(Board&);
};

#endif //ALPHA_BETA_H
