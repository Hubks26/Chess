#ifndef ENGINE_H
#define ENGINE_H

#include "Board.h"

class Engine
{
public :
	
	Engine();
	void run();
    void handleMouseInput(sf::Mouse::Button mouse, bool isPressed);
	bool isInsideWindow(void) const;

private :
	
	void processEvents();
	void update();
	void render();
	
private :
	sf::RenderWindow m_window;
	static const sf::Time m_timePerFrame;
	bool m_mouseL=false, m_mouseLPressed = false;

	char m_chosenPiece;
	sf::Vector2i m_pieceCoord;
	Board m_board;
	sf::Texture m_texture;
	sf::Sprite m_pieceSprite;
};

#endif //ENGINE_H
