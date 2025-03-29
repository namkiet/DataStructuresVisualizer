#pragma once
#include "GUI/Component.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include "vector"

namespace GUI
{
    class TextBox : public Component
    {
    public:
        static const std::string                    mAllowedChars;
        typedef std::shared_ptr<TextBox>            Ptr;

    public:
                                                    TextBox(const sf::Font& font, sf::Vector2f position, sf::Vector2f size, unsigned int charSize = 24);
        void                                        handleEvent(const sf::Event& event) override;
        void                                        draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    public:
        void                                        setColor(sf::Color color);
        void                                        setText(const std::string& text);
        std::string                                 getText() const;
        bool                                        isSelectable() const override { return true; }
        void                                        inputLogic(sf::Uint32 unicode);
        void                                        reset();
        virtual void                                select() override;
        virtual void                                deselect() override;
        int                                        getInputNum();
    private:
        sf::RectangleShape mBox;
        sf::Text mText;
        std::string mInput;
        int InputNum;

        sf::Color                                  mSelectOutlineColor;
        sf::Color                                  mDefaultOutlineColor;
    };
}