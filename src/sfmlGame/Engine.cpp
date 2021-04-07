#include "Engine.h"

const sf::Time Engine::m_timePerFrame = sf::seconds(1.f/24.f);

Engine::Engine()
: m_window(sf::VideoMode(1600, 1600), "Chess")
, m_board()
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
		
		if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			m_window.close();
	}
}

void Engine::update()
{
}

void Engine::render()
{
	m_window.clear();
	m_window.draw(m_board);
	m_window.display();
}
