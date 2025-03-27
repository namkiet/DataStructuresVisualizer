    #pragma once
    #include "Core/ResourceHolder.hpp"
    #include "SFML/Graphics.hpp"
    #include <vector>
    #include <string>
    #include <memory>
    #include <functional>
    #include "Core/ResourceIdentifiers.hpp"
    #include "Core/User.hpp"
    #include<vector>

    #include "GUI/Component.hpp"


namespace GUI
{
    extern const sf::Vector2f ButtonSize;

    class Button: public Component
    {
        public:
            typedef std::shared_ptr<Button>		Ptr;
            typedef std::function<void()>		Callback;


        public:
                                    Button(sf::Font& fonts, sf::Vector2f Position, std::string text);

            std::string                  getText();
            void					setCallback(Callback callback);
            virtual void			handleEvent(const sf::Event& event);

            void			        draw(sf::RenderTarget& target, sf::RenderStates states) const;

            void					setToggle(bool flag);

            virtual bool			isSelectable() const;
            virtual void			select();
            virtual void			deselect();

            void                    activate() override;
            void                    deactivate() override;

        protected:
            Callback				mCallback;
        private:
x
            sf::Text				mText;
            bool                    mIsToggle;
            sf::Color               mNormalColor;
            sf::Color               mSelectedColor;

        protected:
            sf::RectangleShape      mShape;

    };
}





