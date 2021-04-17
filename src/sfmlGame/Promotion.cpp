#include "Promotion.h"

Promotion::Promotion(unsigned int col, Color c, sf::Window& window)
: m_col(col)
, m_color(c)
, m_window(window)
, m_promotionType(Type::None)
{
    m_choices.push_back(new Piece(m_color, Type::QUEEN));
    m_choices.push_back(new Piece(m_color, Type::KNIGHT));
    m_choices.push_back(new Piece(m_color, Type::ROOK));
    m_choices.push_back(new Piece(m_color, Type::BISHOP));
}

void Promotion::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f((target.getSize().x)/8.f, (target.getSize().y)/8.f));

    sf::Sprite sprite;

    if (m_color == Color::WHITE)
    {
        for (std::size_t i = 0; i < 4; ++i)
        {
            sprite = m_choices[i]->getSprite();
            sf::FloatRect rectPiece = sprite.getLocalBounds();
            sprite.setOrigin(rectPiece.width/2.f, rectPiece.height/2.f);

			sprite.setPosition(((m_col+0.5)*target.getSize().x)/8, ((i+0.5)*target.getSize().y)/8);

            rectangle.setPosition((m_col*target.getSize().x)/8, (i*target.getSize().y)/8);
            if ((m_col+i)%2 == 0) {rectangle.setFillColor(sf::Color(240, 217, 181));}
            else {rectangle.setFillColor(sf::Color(181, 136, 99));}
            target.draw(rectangle, states);

            if (mouseIsOnCase(m_col, i))
            {
                sprite.scale(target.getSize().x/(rectPiece.width*8.f), target.getSize().y/(rectPiece.height*8.f));
                rectangle.setFillColor(sf::Color(100, 110, 20, 150));
            }
            else 
            {
                sprite.scale(target.getSize().x/(rectPiece.width*8.f)*0.8, target.getSize().y/(rectPiece.height*8.f)*0.8);
                rectangle.setFillColor(sf::Color(190, 80, 40, 80));
            }
            target.draw(rectangle, states);
            target.draw(sprite, states);
        }
    }
}

bool Promotion::mouseIsOnCase(int col, int i) const
{
    bool flag = false;

    if (sf::Mouse::getPosition(m_window).x > m_col*m_window.getSize().x/8.f && 
        sf::Mouse::getPosition(m_window).x < (m_col+1.f)*m_window.getSize().x/8.f &&
        sf::Mouse::getPosition(m_window).y > i*m_window.getSize().y/8.f && 
        sf::Mouse::getPosition(m_window).y < (i+1.f)*m_window.getSize().y/8.f)
	{
		flag = true;
	}

    return flag;
}