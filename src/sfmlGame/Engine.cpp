#include "Engine.h"

const sf::Time Engine::m_timePerFrame = sf::seconds(1.f/60.f);

Engine::Engine()
: m_window(sf::VideoMode(600, 600), "Chess")
, m_chosenPiece('0')
, m_pieceCoord(0,0)
, m_board()
{
	m_texture.loadFromFile("assets/white/K.png");
	m_pieceSprite.setTexture(m_texture);
	m_texture.setSmooth(true);
	sf::FloatRect rectPiece = m_pieceSprite.getLocalBounds();
	m_pieceSprite.scale(m_window.getSize().x/(rectPiece.width*8.f), m_window.getSize().y/(rectPiece.height*8.f));
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

void Engine::handleMouseInput(sf::Mouse::Button mouse, bool isPressed)
{
    if(mouse == sf::Mouse::Left) { this-> m_mouseL = isPressed; 
		if(!isPressed) { this->m_mouseLPressed=false; }
	}
}

bool Engine::isInsideWindow() const
{
	bool flag = false;
	
	if(sf::Mouse::getPosition(m_window).x>0 && sf::Mouse::getPosition(m_window).x< 600 && 
		sf::Mouse::getPosition(m_window).y>0 && sf::Mouse::getPosition(m_window).y< 600)  //remplacer 600
	{
		flag = true;
	}
	
	return flag;
}


void Engine::update()
{
	std::string directory = "assets/";
	std::string color;
	std::string ext = ".png";
	std::string path;

	if(isInsideWindow())
	{
		if(m_mouseL && !m_mouseLPressed)
		{
			m_chosenPiece = m_board.getPiece(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y);
			m_pieceCoord = sf::Vector2i(sf::Mouse::getPosition(m_window).x*8/600, sf::Mouse::getPosition(m_window).y*8/600);//remplacer 600
			m_board.setPiece(m_pieceCoord.x, m_pieceCoord.y, '0');
			this->m_mouseLPressed=true;
		}

		if(!m_mouseL)
		{
			if(m_chosenPiece != '0')
			{
				m_board.setPiece(sf::Mouse::getPosition(m_window).x*8/600, sf::Mouse::getPosition(m_window).y*8/600, m_chosenPiece);
			}
			m_chosenPiece = '0';
		}
	}
	
	if(m_chosenPiece != '0')
	{
		if(int(m_chosenPiece) <= 90)
		{
			color = "white/";
		}
		else 
		{
			color = "black/";
		}
		
		path = directory + color + m_chosenPiece + ext;
		m_texture.loadFromFile(path);
		m_pieceSprite.setTexture(m_texture);
		m_pieceSprite.setPosition(sf::Mouse::getPosition(m_window).x - 37 ,sf::Mouse::getPosition(m_window).y - 37); //Calculer 37
	}

}

void Engine::render()
{
	m_window.clear();
	m_window.draw(m_board);
	if(m_chosenPiece != '0')
	{
		m_window.draw(m_pieceSprite);
		
	}
	m_window.display();
}
