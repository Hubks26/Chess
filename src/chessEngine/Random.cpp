#include "Random.h"
#include "../sfmlGame/Board.h"

Random::Random()
{
}

Move Random::getMove(Board& board)
{
    ListOfMoves legalMoves = board.allowedMoves();

    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distU(0, legalMoves.size()-1);
	int index = distU(generator);

    Move move = legalMoves[index];

    return move;
}