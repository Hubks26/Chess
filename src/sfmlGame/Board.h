#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

#include "Piece.h"

using ListOfMoves = std::vector<std::tuple<unsigned int, unsigned int, Type>>;

class Board : public sf::Drawable
{
	public :
		
		std::vector<std::string> coord{
			"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
			"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
			"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
			"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
			"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
			"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
			"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
			"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
		};
		
	public :
		Board(std::string fen);
		
		ListOfMoves allMoves(Color=Color::Null, bool=false) const;
		ListOfMoves allowedMoves(Color=Color::Null) const; //Utile ?
		void setPosition(std::string fen);
		std::string getFEN() const;
		void moveAPiece(std::tuple<int, int, Type>);
		bool isMovePossible(std::tuple<int, int, Type>) const;
		void undo();
		void toggleTurn();
		bool isKingUnderAttack(Color) const;
		bool isPieceUnderAttack(unsigned int, Color) const;
		void printBoard() const;
		void printPGN() const;
		float getPositionEval() const;
		
		
	private :
		
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
		
	public :
		
		std::string m_fen;
		std::vector<Piece*> m_cases;
		bool m_whiteToPlay;
		bool m_roqueK, m_roqueQ, m_roquek, m_roqueq;
		int m_caseEnPassant;
		int m_nbRep;
		int m_nbMoves;
		ListOfMoves m_pgn;
		int m_posOfSelectedPiece;
};

#endif //PIECE_H
