#include "Board.h"

Board::Board()
: m_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
{
	deduceTableFromFen();
}

void Board::deduceTableFromFen()
{
	for(std::size_t i = 0; i < 8; ++i)
	{
		for(std::size_t j = 0; j < 8; ++j)
		{
			m_table[i][j] = '0';
		}
	}
	
	std::size_t row = 0;
	std::size_t col = 0;
	std::size_t s = m_fen.size();
	std::string num = "12345678";
	
	for(std::size_t i = 0; i < s; ++i)
	{
		char cell = m_fen[i];
		if(cell == ' ')
			break;
		else if(cell == '/')
		{
			++row;
			col = 0;
		}
		else if(num.find_first_of(cell) < s)
			col += cell - '0';
		else
		{
			m_table[row][col] = cell;
			++col;
		}
	}
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Texture texture;
	texture.loadFromFile("assets/white/K.png");
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.scale(target.getSize().x/1600.f, target.getSize().y/1600.f); // 1600 = 200 * 8 (taille de l'image * nombre de cases)

	std::string directory = "assets/";
	std::string color;
	std::string ext = ".png";
	std::string path;
	
	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f((target.getSize().x+1.f)/8.f, (target.getSize().y+1.f)/8.f));
	
	for (std::size_t i = 0; i < 8; ++i)
	{
		for (std::size_t j = 0; j < 8; ++j)
		{
			rectangle.setPosition((i*target.getSize().x+1.f)/8, (j*target.getSize().y+1.f)/8);
			if((i+j)%2 == 0)
				rectangle.setFillColor(sf::Color(240, 217, 181));
			else
				rectangle.setFillColor(sf::Color(181, 136, 99));
			target.draw(rectangle, states);
			
			std::string pieces = "pnbrqkPNBRQK";
			char cursor = m_table[j][i];
			if(pieces.find_first_of(cursor) < pieces.size())
			{
				if(int(cursor) <= 90)
					color = "white/";
				else
					color = "black/";

				path = directory + color + cursor + ext;
				texture.loadFromFile(path);
				sprite.setPosition((i*target.getSize().x+1.f)/8, (j*target.getSize().y+1.f)/8);
				target.draw(sprite, states);
			}
		}
	}
}

void Board::print() const
{
	std::cout << std::endl;
	std::cout << "\t+---+---+---+---+---+---+---+---+" << std::endl;
	for(std::size_t i = 0; i < 8; ++i)
	{
		std::cout << "\t| ";
		for(std::size_t j = 0; j < 8; ++j)
		{
			std::cout << m_table[i][j] << " | ";
		}
		std::cout << std::endl;
		std::cout << "\t+---+---+---+---+---+---+---+---+" << std::endl;
	}
	std::cout << std::endl;
}
