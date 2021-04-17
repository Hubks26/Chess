#ifndef PROMOTION_H
#define PROMOTION_H

#include <SFML/Graphics.hpp>

#include "Piece.h"

class Promotion : public sf::Drawable
{
    public:
        Promotion(unsigned int, Color, sf::Window&);

    private:
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

        bool mouseIsOnCase(int, int) const;

    private:
        unsigned int m_col;
        Color m_color;
        sf::Window& m_window;
        Type m_promotionType;
        std::vector<Piece*> m_choices;

};

#endif //PROMOTION_H