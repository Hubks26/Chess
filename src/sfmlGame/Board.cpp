#include "Board.h"

Board::Board()
: m_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
{
	deduceTableFromFen();
}

void Board::setPiece(const int x, const int y, const char piece) // x, y en pixels
{
	m_table[y][x] = piece;
}

char Board::getPiece( int x,  int y) const
{
	return m_table[y*8/600][x*8/600];
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
	sf::FloatRect rectSprite = sprite.getLocalBounds();
	sprite.scale(target.getSize().x/(rectSprite.width*8.f), target.getSize().y/(rectSprite.height*8.f));
	texture.setSmooth(true);

	std::string directory = "assets/";
	std::string color;
	std::string ext = ".png";
	std::string path;
	std::string pieces = "pnbrqkPNBRQK";
	
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
			
			char cursor = m_table[j][i];
			if(pieces.find_first_of(cursor) < pieces.size())
			{
				if(int(cursor) <= 90)
					color = "white/";
				else
					color = "black/";

				path = directory + color + cursor + ext;
				texture.loadFromFile(path);
				sprite.setPosition((i*target.getSize().x)/8, (j*target.getSize().y)/8);
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
