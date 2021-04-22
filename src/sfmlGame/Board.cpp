#include "Board.h"

Board::Board(std::string fen)
{
	if (!m_bufferMove.loadFromFile("assets/sounds/son_deplacement.wav"))
        throw std::runtime_error ("Engine::Engine() - Failed to load 'assets/sounds/son_deplacement.wav'");
    m_soundMove.setBuffer(m_bufferMove);
    
    if (!m_bufferTake.loadFromFile("assets/sounds/son_prise.wav"))
        throw std::runtime_error ("Engine::Engine() - Failed to load 'assets/sounds/son_prise.wav'");
    m_soundTake.setBuffer(m_bufferTake);

	setPosition(fen);

	m_gameStates.emplace_back(true, true, true, true, -1);
}

void Board::setPosition(std::string fen)
{
	m_fen = fen;
	m_cases.clear();

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

	m_posOfSelectedPiece = -1;
}

std::string Board::getFEN() const
{
	std::string fen;
	int space = 0;

	for (std::size_t i = 0; i < 64; ++i)
	{
		if (i != 0 && i%8 == 0)
		{
			if (space != 0)
			{
				fen += space + '0';
			}
			fen += '/';
			space = 0;
		}
		if (m_cases[i]->m_code != '-')
		{
			if (space != 0)
			{
				fen += space + '0';
			}
			fen += m_cases[i]->m_code;
			space = 0;
		}
		else
		{
			++space;
		}
	}
	fen += ' ';

	if (m_whiteToPlay) {fen += 'w';}
	else {fen += 'b';}
	fen += ' ';

	if (m_roqueK) {fen += 'K';}
	if (m_roqueQ) {fen += 'Q';}
	if (m_roquek) {fen += 'k';}
	if (m_roqueQ) {fen += 'q';}
	if (!m_roqueK && ! m_roqueQ && !m_roquek && !m_roqueq) {fen += '-';}
	fen += ' ';

	if (m_caseEnPassant == -1) {fen += '-';}
	else {fen += coord[m_caseEnPassant];}
	fen += ' ';

	fen += std::to_string(m_nbRep);
	fen += ' ';
	fen += std::to_string(m_nbMoves);

	return fen;
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f((target.getSize().x)/8.f, (target.getSize().y)/8.f));
	ListOfMoves moves;

	if (m_posOfSelectedPiece >= 0)
	{
		moves = m_cases[m_posOfSelectedPiece]->movesForPiece(m_posOfSelectedPiece, this, (m_whiteToPlay ? Color::WHITE : Color::BLACK));
	}
	
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

			if (m_cases[i + 8*j]->m_type == Type::KING &&
				isPieceUnderAttack(i + j*8, (m_cases[i + 8*j]->m_color == Color::WHITE ? Color::BLACK : Color::WHITE)))
			{
				rectangle.setFillColor(sf::Color(230, 50, 50, 190));
				target.draw(rectangle, states);
			}

			if (m_pgn.size() != 0 && (std::get<0>(m_pgn[m_pgn.size()-1]) == i+8*j || std::get<1>(m_pgn[m_pgn.size()-1]) == i+8*j))
			{
				rectangle.setFillColor(sf::Color(240, 240, 0, 90));
				target.draw(rectangle, states);
			}

			if (m_posOfSelectedPiece != -1 && 
				unsigned(m_posOfSelectedPiece) == i + 8*j &&
				m_cases[m_posOfSelectedPiece]->m_color == (m_whiteToPlay ? Color::WHITE : Color::BLACK))
			{
				rectangle.setFillColor(sf::Color(230, 110, 30, 150));
				target.draw(rectangle, states);
			}
			for (auto it = moves.begin(); it != moves.end(); ++it)
			{
				if (std::get<1>(*it) == i + 8*j &&
					std::get<2>(*it) != Type::BISHOP &&
					std::get<2>(*it) != Type::KNIGHT &&
					std::get<2>(*it) != Type::ROOK)
				{
					for (auto moveIt = m_allowedMoves.begin(); moveIt != m_allowedMoves.end(); ++moveIt)
					{
						if (*it == *moveIt)
						{
							rectangle.setFillColor(sf::Color(130, 200, 70, 150));
							target.draw(rectangle, states);
							break;
						}
					}
				}
			}
			
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

void Board::printHistory() const
{
	for(std::size_t i = 0; i < m_pgn.size(); ++i)
	{
		std::cout << "Move : " << coord[std::get<0>(m_pgn[i])] << "->" << coord[std::get<1>(m_pgn[i])] << " --- ";
		std::cout << (i+1)/2 << std::endl;
	}
	std::cout << std::endl;
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

ListOfMoves Board::allowedMoves(Color c)
{
	ListOfMoves moves;
	ListOfMoves all = allMoves(c);

	if (c == Color::Null)
	{
		if (m_whiteToPlay) {c = Color::WHITE;}
		else {c = Color::BLACK;}
	}

	for (auto it = all.begin(); it != all.end(); ++it)
	{
		if (m_cases[std::get<0>(*it)]->m_type == Type::KING &&
			abs(int(std::get<0>(*it)) - int(std::get<1>(*it))) == 2)
		{
			moves.push_back(*it);
		}
		else
		{
			moveAPiece(*it, true);
			if (!isKingUnderAttack(c))
			{
				moves.push_back(*it);
			}
			undo();
		}
	}
	return moves;
}

void Board::moveAPiece(std::tuple<int, int, Type> move, bool amongAllMoves, bool soundOn, bool needVerification)
{
	int p64 = std::get<0>(move);
	int p64AfterMove = std::get<1>(move);

	if (!needVerification || isMovePossible(move, amongAllMoves))
	{
		if (!amongAllMoves && soundOn)
		{
			if (m_cases[std::get<0>(move)]->m_type == Type::PAWN && std::get<1>(move) == m_caseEnPassant)
				m_soundTake.play();
			else if (m_cases[std::get<1>(move)]->m_type == Type::None)
				m_soundMove.play();
			else
				m_soundTake.play();
		}

		m_pgn.emplace_back(move);

		m_piecesTooked.emplace_back(p64AfterMove, m_cases[p64AfterMove]);

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
			{
				std::get<0>(m_piecesTooked[m_piecesTooked.size() - 1]) = p64AfterMove + 8;
				std::get<1>(m_piecesTooked[m_piecesTooked.size() - 1]) = m_cases[p64AfterMove + 8];
				m_cases[p64AfterMove + 8] = new Piece('-');
			}
			else if (p64AfterMove/8 == 5)
			{
				std::get<0>(m_piecesTooked[m_piecesTooked.size() - 1]) = p64AfterMove - 8;
				std::get<1>(m_piecesTooked[m_piecesTooked.size() - 1]) = m_cases[p64AfterMove - 8];
				m_cases[p64AfterMove - 8] = new Piece('-');
			}
		}

		if (m_cases[p64AfterMove]->m_type == Type::PAWN)
		{
			if (p64/8-p64AfterMove/8 == -2)
				{m_caseEnPassant = p64AfterMove - 8;}
			else if (p64/8-p64AfterMove/8 == 2)
				{m_caseEnPassant = p64AfterMove + 8;}
			else {m_caseEnPassant = -1;}
		}
		else {m_caseEnPassant = -1;}

		if (std::get<2>(move) != Type::None)
		{
			m_cases[p64AfterMove] = new Piece(m_cases[p64AfterMove]->m_color, std::get<2>(move));
		}

		m_gameStates.emplace_back(m_roqueK, m_roqueQ, m_roquek, m_roqueq, m_caseEnPassant);
		toggleTurn();
	}
}

void Board::undo()
{
	if (m_pgn.size() != 0)
	{
		std::size_t s = m_pgn.size();
		int lastPos = std::get<0>(m_pgn[s-1]);
		int newPos = std::get<1>(m_pgn[s-1]);
		m_pgn.pop_back();

		m_roqueK = std::get<0>(m_gameStates[s-1]);
		m_roqueQ = std::get<1>(m_gameStates[s-1]);
		m_roquek = std::get<2>(m_gameStates[s-1]);
		m_roqueq = std::get<3>(m_gameStates[s-1]);
		m_caseEnPassant = std::get<4>(m_gameStates[s-1]);
		m_gameStates.pop_back();

		if (std::get<2>(m_pgn[s-1]) == Type::None)
		{
			m_cases[lastPos] = m_cases[newPos];
			m_cases[newPos] = new Piece('-');
			m_cases[std::get<0>(m_piecesTooked[m_piecesTooked.size() - 1])] = std::get<1>(m_piecesTooked[m_piecesTooked.size() - 1]);

			if (m_cases[lastPos]->m_type == Type::KING)
			{
				if (lastPos == 4 && newPos == 6)
				{
					m_cases[7] = m_cases[lastPos + 1];
					m_cases[lastPos + 1] = new Piece('-');
				}
				else if (lastPos == 4 && newPos == 2)
				{
					m_cases[0] = m_cases[lastPos - 1];
					m_cases[lastPos - 1] = new Piece('-');
				}
				else if (lastPos == 60 && newPos == 62)
				{
					m_cases[63] = m_cases[lastPos + 1];
					m_cases[lastPos + 1] = new Piece('-');
				}
				else if (lastPos == 60 && newPos == 58)
				{
					m_cases[56] = m_cases[lastPos - 1];
					m_cases[lastPos - 1] = new Piece('-');
				}
			}
		}
		else 
		{
			m_cases[lastPos] = new Piece(m_cases[newPos]->m_color, Type::PAWN);
			m_cases[std::get<0>(m_piecesTooked[m_piecesTooked.size() - 1])] = std::get<1>(m_piecesTooked[m_piecesTooked.size() - 1]);
		}

		m_piecesTooked.pop_back();
		toggleTurn();
	}
}

bool Board::isMovePossible(std::tuple<int, int, Type> move, bool amongAllMoves)
{
	bool moveExist = false;
	ListOfMoves moves = amongAllMoves ? allMoves() : allowedMoves();

	for (auto it = moves.begin(); it != moves.end(); ++it)
	{
		if (*it == move)
		{
			moveExist = true;
			break;
		}
	}

	return moveExist;
}

bool Board::isPieceUnderAttack(unsigned int p64, Color c) const
{
	bool flag = false;
	ListOfMoves moves;
	moves = allMoves(c, true);

	for (auto it = moves.begin(); it != moves.end(); ++it)
	{	
		if(std::get<1>(*it) == p64) 
		{
			flag = true;
			break;
		}
	}

	return flag;
}

bool Board::isKingUnderAttack(Color c) const
{
	unsigned int p64 = 0;

	for(int i = 0; i < 64; ++i)
	{
		if(m_cases[i]->m_type == Type::KING && m_cases[i]->m_color == c)
		{
			p64 = i;
			break;
		}
	}
	return isPieceUnderAttack(p64, (c == Color::WHITE ? Color::BLACK : Color::WHITE));
}

void Board::toggleTurn()
{
	m_whiteToPlay = !m_whiteToPlay;
}

void Board::update()
{
	m_allowedMoves = allowedMoves();

	if (m_allowedMoves.size() == 0)
	{
		if (isKingUnderAttack(Color::WHITE))
		{
			std::cout << "Victoire des NOIRS !" << std::endl;
		}
		else if (isKingUnderAttack(Color::BLACK))
		{
			std::cout << "Victoire des BLANCS !" << std::endl;
		}
		else
		{
			std::cout << "C'est un PAT !" << std::endl;
		}
	}
}