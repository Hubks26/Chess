#include "AlphaBeta.h"
#include "../sfmlGame/Board.h"

AlphaBeta::AlphaBeta(Board& board, Color c)
: m_board(board)
, m_color(c)
{
}

int AlphaBeta::eval()
{
    int score = 0;

    for (std::size_t i = 0; i < 64; ++i)
    {
        if (m_board.m_cases[i]->m_color == Color::WHITE)
        {
            score += m_board.m_cases[i]->m_value;
        }
        else if(m_board.m_cases[i]->m_color == Color::BLACK)
        {
            score -= m_board.m_cases[i]->m_value;
        }
    }

    int perspective = m_board.m_whiteToPlay ? 1 : -1;

    return score*perspective;
}

Move AlphaBeta::getMove()
{
    ListOfMoves legalMoves = m_board.allowedMoves();
    int indexBestMove = 0;
    int bestScore = 1000;
    int score;

    for (std::size_t i = 0; i < legalMoves.size(); ++i)
    {
        m_board.moveAPiece(legalMoves[i], false, false, false);
        score = search(3);
        m_board.undo();
        if (score < bestScore)
        {
            bestScore = score;
            indexBestMove = i;
        }
    }

    return legalMoves[indexBestMove];
}

int AlphaBeta::search(int depth, int alpha, int beta)
{
    if (depth == 0)
    {
        return eval();
    }

    ListOfMoves legalMoves = m_board.allowedMoves();

    int evaluation;

    if (legalMoves.size() == 0)
    {
        if (m_board.isKingUnderAttack(m_color))
        {
            return -1000;
        }
        return 0;
    }

    for (auto it = legalMoves.begin(); it != legalMoves.end(); ++it)
    {
        m_board.moveAPiece(*it, false, false, false);
        evaluation = -search(depth - 1, -beta, -alpha);
        m_board.undo();
        if (evaluation >= beta) {return beta;}
        if (evaluation > alpha) {alpha = evaluation;}
    }
    return alpha;
}