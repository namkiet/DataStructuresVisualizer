#include <State/MenuState.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <iostream>

MenuState::MenuState(StateStack& stack, Context context): 
	State(stack, context)
{
	Textures::ID currentBackground = getCurrentBackgroundOption();
	mBackground.setTexture(context.textures->get(currentBackground));
	sf::Color color = mBackground.getColor();
	color.a = 0;
	mBackground.setColor(sf::Color(100, 100, 100));
	// mBackground.setColor(sf::Color::Red);

	if (!mShader.loadFromFile("shader/colormode.frag", sf::Shader::Fragment)){
		// exit(0);
	}


	mCarousel.pushItem(Carousel::Item::AVL, 		"AVLL-modified.png", 	[this](){ requestStackPop(); requestStackPush(States::InAppAVL); });
	mCarousel.pushItem(Carousel::Item::HEAP, 		"AVLL-modified.png", 	[this](){ requestStackPop(); requestStackPush(States::InAppHeap); });
	mCarousel.pushItem(Carousel::Item::LINKED_LIST, "AVLL-modified.png", 	[this](){ requestStackPop(); requestStackPush(States::InAppLinkedList); });
	mCarousel.pushItem(Carousel::Item::GRAPH, 		"AVLL-modified.png", 	[this](){ requestStackPop(); requestStackPush(States::InAppGraph); });

	// mCarousel.pushItem(Carousel::Item::AVL, 		"avl-thumbnail.png", 	[this](){ requestStackPop(); requestStackPush(States::InAppAVL); });
	// mCarousel.pushItem(Carousel::Item::HEAP, 		"heap-thumbnail.png", 	[this](){ requestStackPop(); requestStackPush(States::InAppHeap); });
	// mCarousel.pushItem(Carousel::Item::LINKED_LIST, "ll-thumbnail.png", 	[this](){ requestStackPop(); requestStackPush(States::InAppLinkedList); });
	// mCarousel.pushItem(Carousel::Item::GRAPH, 		"graph-thumbnail.png", 	[this](){ requestStackPop(); requestStackPush(States::InAppGraph); });

	// setting button
	mSettingButton = std::make_shared<GUI::Button>(context.fonts->get(Fonts::UI), sf::Vector2f(SCREEN::Width - 80.f, 20.f), "", sf::Vector2f(50.f, 50.f), GUI::Button::ShapeType::Circle, GUI::Button::ContentType::Image);
	mSettingButton->setNormalColor(sf::Color(255, 255, 255, 50));
	mSettingButton->setSelectedColor(sf::Color(255, 255, 255, 100));
	mSettingButton->setSprite(sf::Sprite(context.textures->get(Textures::SettingIcon)));
	mSettingButton->setCallback([this](){
		requestStackPop();
		requestStackPush(States::Settings);
	});

}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mBackground);
	mCarousel.draw(window);
	window.draw(*mSettingButton);
}

bool MenuState::update(sf::Time dt)
{
	mCarousel.update(dt);
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Left)
			mCarousel.prev();
		if (event.key.code == sf::Keyboard::Right)
			mCarousel.next();
	}
	mSettingButton->handleEvent(event);
	mCarousel.handleEvent(event);
	return true;
}