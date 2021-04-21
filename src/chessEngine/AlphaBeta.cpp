#include "AlphaBeta.h"
#include "../sfmlGame/Board.h"

AlphaBeta::AlphaBeta()
{
}

int AlphaBeta::eval(Board& board)
{
    int score = 0;

    for (std::size_t i = 0; i < 64; ++i)
    {
        if (board.m_cases[i]->m_color == Color::WHITE)
        {
            score += board.m_cases[i]->m_value;
        }
        else if(board.m_cases[i]->m_color == Color::BLACK)
        {
            score -= board.m_cases[i]->m_value;
        }
    }

    int perspective = (board.m_whiteToPlay) ? 1 : -1;

    return score*perspective;
}

Move AlphaBeta::getMove(Board& board)
{
    ListOfMoves legalMoves = board.allowedMoves();

    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distU(0, legalMoves.size()-1);
	int index = distU(generator);

    Move move = legalMoves[index];

    return move;
}