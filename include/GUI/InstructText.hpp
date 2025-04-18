#pragma once

#include<SFML/Graphics.hpp>
#include <memory>
namespace GUI{
    class InstructText
    {
        public:
            typedef std::shared_ptr<sf::Text> Ptr;
        public:
            InstructText::Ptr mInstruct;
            bool              hasInfo;
        public:
            InstructText();
            void			  draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };
}