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
        void                                        setPlaceholderPosition(sf::Vector2f pos);
        void                                        submit();
        sf::FloatRect                               getGlobalBounds() const;
        void                                        setMessage(std::string message);
        void                                        setTextPosition(sf::Vector2f pos);
        void                                        setAllowedEndLine(bool isAllowed);

    public:
        InputType                                   mInputType;
        sf::Text                                    mText;
        unsigned int                                mCharSize;
        sf::Text                                    mPlaceholder;
    private:

        sf::RectangleShape                          mBox;

        std::string                                 mInput;
        int                                         InputNum;
        std::vector<int>                            InputNumList;
        sf::Text                                    mMessage;
        bool                                        showMessage;
        bool                                        mAllowedEndline;


    private:
        sf::Color                                   mSelectOutlineColor;
        sf::Color                                   mDefaultOutlineColor;
        bool                                        showCursor;
        sf::Clock                                   cursorTimer;
    };
}