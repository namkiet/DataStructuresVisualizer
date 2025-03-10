    #pragma once
    #include "ResourceHolder.hpp"
    #include "SFML/Graphics.hpp"
    #include <vector>
    #include <string>
    #include <memory>
    #include <functional>
    #include "Core/ResourceIdentifiers.hpp"
    #include "Core/User.hpp"
    #include<vector>


    extern const sf::Vector2f ButtonSize;

    class Button: public sf::Drawable, public sf::Transformable
    {
        public:
            typedef std::shared_ptr<Button>		Ptr;


        public:
                                    Button(const FontHolder& fonts, const TextureHolder& textures, sf::Vector2f Position);

            void					setText(const std::string& text);

            virtual void			handleEvent(const sf::Event& event);

            void                    setAction(User::Action action);

            User::Action            getAction();

            void			        draw(sf::RenderTarget& target, sf::RenderStates states) const;


        private:
            sf::Sprite              mSprite;
            User::Action            action;
            sf::Text				mText;
    };




