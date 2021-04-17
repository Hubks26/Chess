#include "Board.h"

Board::Board(std::string fen)
{
	m_fen = fen;
	std::size_t s = m_fen.size();
	std::vector<std::string> vectFen;
	std::string num = "12345678";
	char cell;
	
	vectFen.push_back("");
	
	for(std::size_t i = 0; i < s; ++i)
	{
		cell = m_fen[i];
		if(cell == ' ')
		{
			vectFen.push_back("");
		}
		else
		{
			vectFen[vectFen.size()-1] += cell;
		}
	}
	
	for(std::size_t i = 0; i < vectFen[0].size(); ++i)
	{
		cell = vectFen[0][i];
		if(num.find_first_of(cell) < num.size())
		{
			for (std::size_t j = 0; j < unsigned(cell - '0'); ++j)
			{	
				m_cases.push_back(new Piece('-'));
			}
		}
		else if(cell != '/')
		{
			m_cases.push_back(new Piece(cell));
		}
	}
	
	m_whiteToPlay = (vectFen[1] == "w");
	
	m_roqueK = vectFen[2].find_first_of('K') < vectFen[2].size();
	m_roqueQ = vectFen[2].find_first_of('Q') < vectFen[2].size();
	m_roquek = vectFen[2].find_first_of('k') < vectFen[2].size();
	m_roqueq = vectFen[2].find_first_of('q') < vectFen[2].size();
	
	m_caseEnPassant = -1;
	if (vectFen[3] != "-")
	{
		for (std::size_t i = 0; i < coord.size(); ++i)
		{
			if (vectFen[3] == coord[i])
				m_caseEnPassant = i;
		}
	}
	
	m_nbRep = std::stoi(vectFen[4]);
	m_nbMoves = std::stoi(vectFen[5]);
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f((target.getSize().x)/8.f, (target.getSize().y)/8.f));
	
	for (std::size_t i = 0; i < 8; ++i)
	{
		for (std::size_t j = 0; j < 8; ++j)
		{
			rectangle.setPosition((i*target.getSize().x)/8, (j*target.getSize().y)/8);
			if((i+j)%2 == 0)
				rectangle.setFillColor(sf::Color(240, 217, 181));
			else
				rectangle.setFillColor(sf::Color(181, 136, 99));
			target.draw(rectangle, states);
			
			sf::Sprite sprite = m_cases[i+j*8]->getSprite();
			sf::FloatRect rectPiece = sprite.getLocalBounds();
			sprite.scale(target.getSize().x/(rectPiece.width*8.f), target.getSize().y/(rectPiece.height*8.f));
			sprite.setPosition((i*target.getSize().x)/8, (j*target.getSize().y)/8);
			target.draw(sprite, states);
		}
	}
}

void Board::printBoard() const
{
	for(std::size_t i = 0; i < m_cases.size(); ++i)
	{
		if(i%8 == 0)
		{
			std::cout << std::endl;
			std::cout << "\t+---+---+---+---+---+---+---+---+" << std::endl;
			std::cout << "\t| ";
		}
		std::cout << m_cases[i]->m_code << " | ";
	}	
	std::cout << std::endl;
	std::cout << "\t+---+---+---+---+---+---+---+---+" << std::endl;
	std::cout << std::endl;
	
	if (m_whiteToPlay)
		std::cout << " White to play" << std::endl;
	else std::cout << " Black to play" << std::endl;
}

ListOfMoves Board::allMoves(Color c, bool dontCallIsAttacked) const
{
	ListOfMoves moves;
	ListOfMoves pieceMoves;

	if (c == Color::Null)
	{
		if (m_whiteToPlay) {c = Color::WHITE;}
		else {c = Color::BLACK;}
	}
	
	for (int p64 = 0; p64 < 64; ++p64)
	{
		if (m_cases[p64]->m_color != c) {continue;}
		if (m_cases[p64]->m_type == Type::KING) 
		{
			pieceMoves = m_cases[p64]->movesForKing(p64, this, c, dontCallIsAttacked);
			moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
			continue;
		}
		if (m_cases[p64]->m_type == Type::QUEEN)
		{
			pieceMoves = m_cases[p64]->movesForRook(p64, this, c);
			moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
			pieceMoves = m_cases[p64]->movesForBishop(p64, this, c);
			moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
			continue;
		}
		if (m_cases[p64]->m_type == Type::ROOK) 
		{
			pieceMoves = m_cases[p64]->movesForRook(p64, this, c);
			moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
			continue;
		}
		if (m_cases[p64]->m_type == Type::BISHOP) 
		{
			pieceMoves = m_cases[p64]->movesForBishop(p64, this, c);
			moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
			continue;
		}
		if (m_cases[p64]->m_type == Type::KNIGHT) 
		{
			pieceMoves = m_cases[p64]->movesForKnight(p64, this, c);
			moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
			continue;
		}
		if (m_cases[p64]->m_type == Type::PAWN) 
		{
			pieceMoves = m_cases[p64]->movesForPawn(p64, this, c);
			moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
			continue;
		}
	}

	return moves;
}

void Board::moveAPiece(std::tuple<int, int, Type> move)
{
	bool moveExist = false;
	int p64 = std::get<0>(move);
	int p64AfterMove = std::get<1>(move);
	//Type promotion = std::get<2>(move);
	ListOfMoves moves = allMoves();
	for (auto it = moves.begin(); it != moves.end(); ++it)
	{
		if (*it == move)
		{
			moveExist = true;
			break;
		}
	}

	if (moveExist)
	{
		m_cases[p64AfterMove] = m_cases[p64];
		m_cases[p64] = new Piece('-');

		if (move == std::tuple<int, int, Type>(4, 6, Type::None) &&
			m_cases[6]->m_type == Type::KING)
		{
			m_cases[5] = m_cases[7];
			m_cases[7] = new Piece('-');
		}
		else if	(move == std::tuple<int, int, Type>(4, 2, Type::None) &&
					m_cases[2]->m_type == Type::KING)
		{
			m_cases[3] = m_cases[0];
			m_cases[0] = new Piece('-');
		}
		else if	(move == std::tuple<int, int, Type>(60, 62, Type::None) &&
					m_cases[62]->m_type == Type::KING)
		{
			m_cases[61] = m_cases[63];
			m_cases[63] = new Piece('-');
		}
		else if	(move == std::tuple<int, int, Type>(60, 58, Type::None) &&
					m_cases[58]->m_type == Type::KING)
		{
			m_cases[59] = m_cases[56];
			m_cases[56] = new Piece('-');
		}

		if (m_cases[p64AfterMove]->m_type == Type::KING)
		{
			if (m_cases[p64AfterMove]->m_color == Color::WHITE) 
			{
				m_roqueK = false;
				m_roqueQ = false;
			}
			else
			{
				m_roquek = false;
				m_roqueq = false;
			}
		}
		if (p64 == 0) {m_roqueq = false;}
		if (p64 == 7) {m_roquek = false;}
		if (p64 == 56) {m_roqueQ = false;}
		if (p64 == 63) {m_roqueK = false;}

		if (m_cases[p64AfterMove]->m_type == Type::PAWN &&
			p64AfterMove == m_caseEnPassant)
		{
			if (p64AfterMove/8 == 2)
				{m_cases[p64AfterMove + 8] = new Piece('-');}
			else if (p64AfterMove/8 == 5)
				{m_cases[p64AfterMove - 8] = new Piece('-');}
		}

		if (m_cases[p64AfterMove]->m_type == Type::PAWN)
		{
			if (p64/8-p64AfterMove/8 == -2)
				{m_caseEnPassant = p64AfterMove - 8;}
			else if (p64/8-p64AfterMove/8 == 2)
				{m_caseEnPassant = p64AfterMove + 8;}
		}
		else {m_caseEnPassant = -1;}

		if (std::get<2>(move) != Type::None)
		{
			m_cases[p64AfterMove] = new Piece(m_cases[p64AfterMove]->m_color, std::get<2>(move));
		}

		toggleTurn();
	}
}

bool Board::isPieceUnderAttack(unsigned int p64, Color c) const
{
	bool flag = false;
	ListOfMoves moves;
	moves = allMoves(c, true);

	for (auto it = moves.begin(); it != moves.end(); ++it)
	{
		if(std::get<1>(*it) == p64) {flag = true;}
	}

	return flag;
}

bool Board::isKingUnderAttack(Color c) const
{
	unsigned int p64 = -1;
	for(int i = 0; i < 64; ++i)
	{
		if(m_cases[i]->m_type == Type::KING && m_cases[i]->m_color == c)
		{
			p64 = i;
			break;
		}
	}

	return isPieceUnderAttack(p64, c);
}

void Board::toggleTurn()
{
	m_whiteToPlay = !m_whiteToPlay;
}