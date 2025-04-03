#pragma once
#include "GUI/Component.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include "vector"
#include "GUI/Button.hpp"

namespace GUI
{
    class ExpandableButton;
    class DeliverTextBox : public Component
    {
    public:
        static const std::string                    mAllowedChars;
        typedef std::shared_ptr<DeliverTextBox>            Ptr;

    public:
                                                    DeliverTextBox(const sf::Font& font, sf::Vector2f position, sf::Vector2f size, unsigned int charSize = 24);
        void                                        handleEvent(const sf::Event& event) override;
        void                                        draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        // virtual void                                setCallback(Callback func) override;
    public:
        void                                        setColor(sf::Color color);
        void                                        setText(const std::string& text);
        std::string                                 getText() const;
        bool                                        isSelectable() const override { return true; }
        // virtual void                                setCallback(Callback callback);
        void                                        inputLogic(sf::Uint32 unicode);
        void                                        reset();
        void                                        setButtonParent(std::shared_ptr<ExpandableButton> Button);
        virtual void                                select() override;
        virtual void                                deselect() override;
        void                                        setInfoID(int id);
    private:
        sf::RectangleShape mBox;
        sf::Text mText;
        std::string mInput;
        int InputNum;
        std::shared_ptr<ExpandableButton>          ButtonParent;
        int                                        mInfoID;

        sf::Color                                  mSelectOutlineColor;
        sf::Color                                  mDefaultOutlineColor;
    };
}