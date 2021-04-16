#include "Piece.h"
#include "Board.h"

Piece::Piece(char c)
{	
	std::string directory = "assets/";
	std::string colorStr;
	std::string ext = ".png";
	std::string path;
	char c_min;
	
	m_code = c;
	
	if (c == '-')
	{
		m_color = Color::Null;
		c_min = c;
	}
	else if (int(c) < 90)
	{
		m_color = Color::WHITE;
		c_min = c + 32;
	}
	else
	{
		m_color = Color::BLACK;
		c_min = c;
	}
	
	if (m_color == Color::WHITE)
	{
		colorStr = "white/";
	}
	else if (m_color == Color::BLACK)
	{
		colorStr = "black/";
	}
	
	switch (c_min)
	{
		case 'p':
			m_type = Type::PAWN;
			m_value = 1;
			break;
			
		case 'n':
			m_type = Type::KNIGHT;
			m_value = 3;
			break;
			
		case 'b':
			m_type = Type::BISHOP;
			m_value = 3;
			break;
			
		case 'r':
			m_type = Type::ROOK;
			m_value = 5;
			break;
			
		case 'q':
			m_type = Type::QUEEN;
			m_value = 9;
			
			break;
			
		case 'k':
			m_type = Type::KING;
			m_value = 1000;
			break;
			
		default:
			m_type = Type::None;
			break;
	}
	
	if (m_type != Type::None)
	{
		path = directory + colorStr + c_min + ext;
		m_texture.loadFromFile(path);
		m_texture.setSmooth(true);
		m_sprite.setTexture(m_texture);
	}
}

sf::Sprite Piece::getSprite() const
{
	return m_sprite;
}


/*#################################################################################################*/
/*#                             Coups possibles pour les pièces                                   #*/
/*#################################################################################################*/


ListOfMoves Piece::movesForKing(int p64, const Board* board, Color c, bool dontCallIsAttacked) const
{
	int p120 = p64 + 21 + 2*(p64/8);
	ListOfMoves liste;
	int n;

	std::vector<int> deplacements;
	deplacements.insert(deplacements.end(), deplacementsTour.begin(), deplacementsTour.end());
	deplacements.insert(deplacements.end(), deplacementsFou.begin(), deplacementsFou.end());

	for (auto it = deplacements.begin(); it != deplacements.end(); ++it)
	{
		n = tab120[p120 + *it];
		if (n>=0 && (board->m_cases[n]->m_type == Type::None || board->m_cases[n]->m_color != c))
		{
			liste.push_back(std::tuple<int, int, Type>(p64, n, Type::None));
		}
	}

	if(!dontCallIsAttacked)
	{
		if (c == Color::WHITE)
		{
			if (board->m_roqueK &&
				board->m_cases[63]->m_type == Type::ROOK &&
				board->m_cases[63]->m_color == c &&
				board->m_cases[62]->m_type == Type::None &&
				board->m_cases[61]->m_type == Type::None &&
				!board->isPieceUnderAttack(62, Color::BLACK) &&
				!board->isPieceUnderAttack(61, Color::BLACK) &&
				!board->isPieceUnderAttack(p64, Color::BLACK))
			{liste.push_back(std::tuple<int, int, Type>(p64, 62, Type::None));}

			if (board->m_roqueQ &&
				board->m_cases[56]->m_type == Type::ROOK &&
				board->m_cases[56]->m_color == c &&
				board->m_cases[57]->m_type == Type::None &&
				board->m_cases[58]->m_type == Type::None &&
				board->m_cases[59]->m_type == Type::None &&
				!board->isPieceUnderAttack(58, Color::BLACK) &&
				!board->isPieceUnderAttack(59, Color::BLACK) &&
				!board->isPieceUnderAttack(p64, Color::BLACK))
			{liste.push_back(std::tuple<int, int, Type>(p64, 58, Type::None));}
		}
		else if (c == Color::BLACK)
		{
			if (board->m_roquek &&
				board->m_cases[7]->m_type == Type::ROOK &&
				board->m_cases[7]->m_color == c &&
				board->m_cases[5]->m_type == Type::None &&
				board->m_cases[6]->m_type == Type::None &&
				!board->isPieceUnderAttack(5, Color::WHITE) &&
				!board->isPieceUnderAttack(6, Color::WHITE) &&
				!board->isPieceUnderAttack(p64, Color::WHITE))
			{liste.push_back(std::tuple<int, int, Type>(p64, 6, Type::None));}

			if (board->m_roqueq &&
				board->m_cases[0]->m_type == Type::ROOK &&
				board->m_cases[0]->m_color == c &&
				board->m_cases[1]->m_type == Type::None &&
				board->m_cases[2]->m_type == Type::None &&
				board->m_cases[3]->m_type == Type::None &&
				!board->isPieceUnderAttack(2, Color::WHITE) &&
				!board->isPieceUnderAttack(3, Color::WHITE) &&
				!board->isPieceUnderAttack(p64, Color::WHITE))
			{liste.push_back(std::tuple<int, int, Type>(p64, 2, Type::None));}
		}
	}

	return liste;
}

ListOfMoves Piece::movesForKnight(int p64, const Board* board, Color c) const
{
	int p120 = p64 + 21 + 2*(p64/8);
	ListOfMoves liste;
	int n;

	for (auto it = deplacementsCavalier.begin(); it != deplacementsCavalier.end(); ++it)
	{
		n = tab120[p120 + *it];
		if (n>=0 && (board->m_cases[n]->m_type == Type::None || board->m_cases[n]->m_color != c))
		{
			liste.push_back(std::tuple<int, int, Type>(p64, n, Type::None));
		}
	}
	return liste;
}

ListOfMoves Piece::movesForRook(int p64, const Board* board, Color c) const
{
	int p120 = p64 + 21 + 2*(p64/8);
	ListOfMoves liste;
	int n, k;

	for (auto it = deplacementsTour.begin(); it != deplacementsTour.end(); ++it)
	{
		k = 1;
		while(true)
		{
			n = tab120[p120 + k*(*it)];
			if (n>=0)
			{
				if (board->m_cases[n]->m_type == Type::None || board->m_cases[n]->m_color != c)
				{
					liste.push_back(std::tuple<int, int, Type>(p64, n, Type::None));
				}
			}
			else {break;}
			if (board->m_cases[n]->m_type != Type::None) {break;}
			++k;
		}
	}
	return liste;
}

ListOfMoves Piece::movesForBishop(int p64, const Board* board, Color c) const
{
	int p120 = p64 + 21 + 2*(p64/8);
	ListOfMoves liste;
	int n, k;

	for (auto it = deplacementsFou.begin(); it != deplacementsFou.end(); ++it)
	{
		k = 1;
		while (true)
		{
			n = tab120[p120 + k*(*it)];
			if (n>=0)
			{
				if (board->m_cases[n]->m_type == Type::None || board->m_cases[n]->m_color != c)
				{
					liste.push_back(std::tuple<int, int, Type>(p64, n, Type::None));
				}
			}
			else {break;}
			if (board->m_cases[n]->m_type != Type::None) {break;}
			++k;
		}
	}
	return liste;
}

ListOfMoves Piece::movesForPawn(int p64, const Board* board, Color c) const
{
	int p120 = p64 + 21 + 2*(p64/8);
	ListOfMoves liste;
	int n;

	//-----------------------------------WHITE PAWN -----------------------------------
	if (c == Color::WHITE)
	{
		n = tab120[p120 - 10];
		if (n >= 0 && board->m_cases[n]->m_type == Type::None)
		{
			if(n < 8)
			{
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::QUEEN));
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::ROOK));
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::KNIGHT));
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::BISHOP));
			}
			else
			{
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::None));
			}
		}

		//Starting Square
		if (p64/8 == 6 &&
			board->m_cases[p64-8]->m_type == Type::None &&
			board->m_cases[p64-16]->m_type == Type::None)
		{
			liste.push_back(std::tuple<int, int, Type>(p64, p64-16, Type::None));
		}

		//Capture upper left
		n = tab120[p120 - 11];
		if (n >= 0 && (board->m_cases[n]->m_color == Color::BLACK || n == board->m_caseEnPassant))
		{
			if(n < 8)
			{
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::QUEEN));
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::ROOK));
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::KNIGHT));
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::BISHOP));
			}
			else
			{
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::None));
			}
		}

		//Capture upper right
		n = tab120[p120 - 9];
		if (n >= 0 && (board->m_cases[n]->m_color == Color::BLACK || n == board->m_caseEnPassant))
		{
			if(n < 8)
			{
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::QUEEN));
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::ROOK));
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::KNIGHT));
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::BISHOP));
			}
			else
			{
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::None));
			}
		}
	}
	//-----------------------------------BLACK PAWN -----------------------------------
	if (c == Color::BLACK)
	{
		n = tab120[p120 + 10];
		if (n >= 0 && board->m_cases[n]->m_type == Type::None)
		{
			if(n > 55)
			{
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::QUEEN));
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::ROOK));
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::KNIGHT));
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::BISHOP));
			}
			else
			{
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::None));
			}
		}

		//Starting Square
		if (p64/8 == 1 &&
			board->m_cases[p64+8]->m_type == Type::None &&
			board->m_cases[p64+16]->m_type == Type::None)
		{
			liste.push_back(std::tuple<int, int, Type>(p64, p64+16, Type::None));
		}

		//Capture bottom left
		n = tab120[p120 + 9];
		if (n >= 0 && (board->m_cases[n]->m_color == Color::WHITE || n == board->m_caseEnPassant))
		{
			if(n >55)
			{
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::QUEEN));
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::ROOK));
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::KNIGHT));
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::BISHOP));
			}
			else
			{
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::None));
			}
		}

		//Capture bottom right
		n = tab120[p120 + 11];
		if (n >= 0 && (board->m_cases[n]->m_color == Color::WHITE || n == board->m_caseEnPassant))
		{
			if(n > 55)
			{
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::QUEEN));
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::ROOK));
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::KNIGHT));
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::BISHOP));
			}
			else
			{
				liste.push_back(std::tuple<int, int, Type>(p64, n, Type::None));
			}
		}
	}
	//---------------------------------------------------------------------------------

	return liste;
}