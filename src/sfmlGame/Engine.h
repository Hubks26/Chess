#ifndef ENGINE_H
#define ENGINE_H

#include "Board.h"
#include "Promotion.h"
#include "../chessEngine/Random.h"

class Engine
{
public :
	
	Engine();
	void run();

private :
	
	void processEvents();
	void update();
	void render();

	void handleMouseInput(sf::Mouse::Button, bool);
	bool isInsideWindow() const;
	int deduceCaseFromMousePosition() const;
	Type deducePromotionFromMousePosition() const;
	
private :
	sf::RenderWindow m_window;
	static const sf::Time m_timePerFrame;
	
	Board m_board;
	int m_posOfSelectedPiece;
	bool m_mouseL, m_mouseLPressed;
	sf::Sprite m_pieceSprite;
	Promotion m_promotionDisp;
	int m_pawnPositionBeforePromotion;
};

#endif //ENGINE_H
