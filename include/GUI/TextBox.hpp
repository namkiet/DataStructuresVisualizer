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
        enum class InputType{
            Number,
            VectorNum,
        };
    public:
        static const std::string                    mAllowedChars;
        typedef std::shared_ptr<TextBox>            Ptr;

    public:
                                                    TextBox(const sf::Font& font, sf::Vector2f position, sf::Vector2f size, std::string placeholder = "", InputType inputType = InputType::Number);
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

        int                                         getInputNum();
        std::vector<int>                            getInputNumList();
        void                                        setPlaceholder(const std::string& text);
        void                                        submit();
        sf::FloatRect                               getGlobalBounds() const;
        void                                        setMessage(std::string message);

    public:
        InputType                                   mInputType;
    private:
        unsigned int                                mCharSize;
        sf::RectangleShape                          mBox;
        sf::Text                                    mText;
        sf::Text                                    mPlaceholder;
        std::string                                 mInput;
        int                                         InputNum;
        std::vector<int>                            InputNumList;
        sf::Text                                    mMessage;
        bool                                        showMessage;


    private:
        sf::Color                                   mSelectOutlineColor;
        sf::Color                                   mDefaultOutlineColor;
        bool                                        showCursor;
        sf::Clock                                   cursorTimer;
    };
}