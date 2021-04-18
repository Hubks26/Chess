#ifndef PROMOTION_H
#define PROMOTION_H

#include <SFML/Graphics.hpp>

#include "Piece.h"

class Promotion : public sf::Drawable
{
    public:
        Promotion(unsigned int, Color, sf::Window&);

        void open();

    private:
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

        bool mouseIsOnCase(int, int) const;

    public:
        unsigned int m_col;
        Color m_color;

    private:
        sf::Window& m_window;
        std::vector<Piece*> m_choices;
        bool m_open;

};

#endif //PROMOTION_H