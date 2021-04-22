#include "Random.h"
#include "../sfmlGame/Board.h"

Random::Random(Board& board)
: m_board(board)
{
}

Move Random::getMove()
{
    ListOfMoves legalMoves = m_board.allowedMoves();
    int index = 0;
    
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distU(0, legalMoves.size()-1);
	index = distU(generator);

    Move move = legalMoves[index];

    return move;
}