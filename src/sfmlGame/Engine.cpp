#include "Engine.h"

const sf::Time Engine::m_timePerFrame = sf::seconds(1.f/120.f);

Engine::Engine()
: m_window(sf::VideoMode(656, 656), "Chess")
, m_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
, m_posOfSelectedPiece(-1)
, m_mouseL(false)
, m_mouseLPressed(false)
, m_promotionDisp(0, Color::Null, m_window)
, m_pawnPositionBeforePromotion(-1)
{
}

void Engine::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (m_window.isOpen())
	{
		processEvents();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > m_timePerFrame)
		{
			timeSinceLastUpdate -= m_timePerFrame;
			update();
		}
		render();
	}
}

void Engine::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		switch(event.type)
		{
			case sf::Event::MouseButtonPressed:
				handleMouseInput(event.mouseButton.button, true);
				break;

			case sf::Event::MouseButtonReleased:
				handleMouseInput(event.mouseButton.button, false);
				break;

			case sf::Event::Closed:
				m_window.close();
				break;
				
			default:
				break;
		}
	}
}

void Engine::update()
{
	int pAfterMove;

	if (m_promotionDisp.isOpen() && isInsideWindow())
	{
		if (m_mouseL)
		{
			m_posOfSelectedPiece = deduceCaseFromMousePosition();
			if (m_posOfSelectedPiece != -1 && unsigned(m_posOfSelectedPiece) == m_promotionDisp.m_col &&
				m_promotionDisp.m_color == Color::WHITE)
			{
				m_board.moveAPiece(std::tuple<int, int, Type>(m_pawnPositionBeforePromotion, m_promotionDisp.m_col, Type::QUEEN));
			}
			else if (m_posOfSelectedPiece != -1 && unsigned(m_posOfSelectedPiece) == m_promotionDisp.m_col + 8 &&
				m_promotionDisp.m_color == Color::WHITE)
			{
				m_board.moveAPiece(std::tuple<int, int, Type>(m_pawnPositionBeforePromotion, m_promotionDisp.m_col, Type::KNIGHT));
			}
			else if (m_posOfSelectedPiece != -1 && unsigned(m_posOfSelectedPiece) == m_promotionDisp.m_col + 16 &&
				m_promotionDisp.m_color == Color::WHITE)
			{
				m_board.moveAPiece(std::tuple<int, int, Type>(m_pawnPositionBeforePromotion, m_promotionDisp.m_col, Type::ROOK));
			}
			else if (m_posOfSelectedPiece != -1 && unsigned(m_posOfSelectedPiece) == m_promotionDisp.m_col + 24 &&
				m_promotionDisp.m_color == Color::WHITE)
			{
				m_board.moveAPiece(std::tuple<int, int, Type>(m_pawnPositionBeforePromotion, m_promotionDisp.m_col, Type::BISHOP));
			}
			else if (m_posOfSelectedPiece != -1 && unsigned(m_posOfSelectedPiece) == m_promotionDisp.m_col + 32 &&
				m_promotionDisp.m_color == Color::BLACK)
			{
				m_board.moveAPiece(std::tuple<int, int, Type>(m_pawnPositionBeforePromotion, m_promotionDisp.m_col+7*8, Type::BISHOP));
			}
			else if (m_posOfSelectedPiece != -1 && unsigned(m_posOfSelectedPiece) == m_promotionDisp.m_col + 40 &&
				m_promotionDisp.m_color == Color::BLACK)
			{
				m_board.moveAPiece(std::tuple<int, int, Type>(m_pawnPositionBeforePromotion, m_promotionDisp.m_col+7*8, Type::ROOK));
			}
			else if (m_posOfSelectedPiece != -1 && unsigned(m_posOfSelectedPiece) == m_promotionDisp.m_col + 48 &&
				m_promotionDisp.m_color == Color::BLACK)
			{
				m_board.moveAPiece(std::tuple<int, int, Type>(m_pawnPositionBeforePromotion, m_promotionDisp.m_col+7*8, Type::KNIGHT));
			}
			else if (m_posOfSelectedPiece != -1 && unsigned(m_posOfSelectedPiece) == m_promotionDisp.m_col + 56 &&
				m_promotionDisp.m_color == Color::BLACK)
			{
				m_board.moveAPiece(std::tuple<int, int, Type>(m_pawnPositionBeforePromotion, m_promotionDisp.m_col+7*8, Type::QUEEN));
			}
			m_promotionDisp.close();
			m_posOfSelectedPiece = -1;
			m_pawnPositionBeforePromotion = -1;
		}
	}
	else
	{
		if (isInsideWindow())
		{
			if(m_mouseL && !m_mouseLPressed)
			{
				m_posOfSelectedPiece = deduceCaseFromMousePosition();
				m_board.m_posOfSelectedPiece = m_posOfSelectedPiece;
				this->m_mouseLPressed=true;
			}
			if(m_posOfSelectedPiece != -1 && !m_mouseL)
			{
				pAfterMove = deduceCaseFromMousePosition();

				if (m_board.m_cases[m_posOfSelectedPiece]->m_type == Type::PAWN)
				{
					if (m_board.m_cases[m_posOfSelectedPiece]->m_color == Color::WHITE &&
						pAfterMove/8 == 0 &&
						m_board.isMovePossible(std::tuple<int, int, Type>(m_posOfSelectedPiece, pAfterMove, Type::QUEEN)))
					{	
						m_promotionDisp.m_col = pAfterMove%8;
						m_promotionDisp.m_color = Color::WHITE;
						m_promotionDisp.open();
						m_pawnPositionBeforePromotion = m_posOfSelectedPiece;
					}
					else if (m_board.m_cases[m_posOfSelectedPiece]->m_color == Color::BLACK &&
						pAfterMove/8 == 7 &&
						m_board.isMovePossible(std::tuple<int, int, Type>(m_posOfSelectedPiece, pAfterMove, Type::QUEEN)))
					{
						m_promotionDisp.m_col = pAfterMove%8;
						m_promotionDisp.m_color = Color::BLACK;
						m_promotionDisp.open();
						m_pawnPositionBeforePromotion = m_posOfSelectedPiece;
					}
				}

				m_board.moveAPiece(std::tuple<int, int, Type>(m_posOfSelectedPiece, pAfterMove, Type::None));
				m_posOfSelectedPiece = -1;
				m_board.m_posOfSelectedPiece = -1;
			}
		}
	}

	if (m_posOfSelectedPiece != -1)
	{
		m_pieceSprite = m_board.m_cases[m_posOfSelectedPiece]->getSprite();
		sf::FloatRect rectPiece = m_pieceSprite.getLocalBounds();
		m_pieceSprite.scale(m_window.getSize().x/(rectPiece.width*8.f)*0.7, m_window.getSize().y/(rectPiece.height*8.f)*0.7);
		m_pieceSprite.setOrigin({rectPiece.width/2.f, rectPiece.height/2.f});
		m_pieceSprite.setPosition(sf::Mouse::getPosition(m_window).x ,sf::Mouse::getPosition(m_window).y);
	}
}

void Engine::render()
{
	m_window.clear();
	m_window.draw(m_board);
	if (m_posOfSelectedPiece != -1 && 
		m_board.m_whiteToPlay == (m_board.m_cases[m_posOfSelectedPiece]->m_color == Color::WHITE))
	{
		m_window.draw(m_pieceSprite);
	}
	m_window.draw(m_promotionDisp);
	m_window.display();
}

void Engine::handleMouseInput(sf::Mouse::Button mouse, bool isPressed)
{
	if(mouse == sf::Mouse::Left) 
	{ 
		this-> m_mouseL = isPressed; 
		if(!isPressed) { this->m_mouseLPressed=false; }
	}
}

bool Engine::isInsideWindow() const
{
	bool flag = false;

	int winX = m_window.getSize().x;
	int winY = m_window.getSize().y;

	if (sf::Mouse::getPosition(m_window).x>0 && sf::Mouse::getPosition(m_window).x< winX &&
		sf::Mouse::getPosition(m_window).y>0 && sf::Mouse::getPosition(m_window).y< winY)
	{
		flag = true;
	}

	return flag;
}

int Engine::deduceCaseFromMousePosition() const
{
	int x = int(sf::Mouse::getPosition(m_window).x*8/m_window.getSize().x);
	int y = int(sf::Mouse::getPosition(m_window).y*8/m_window.getSize().y);
	return x + 8*y;
}