#include <State/MenuState.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <iostream>

MenuState::MenuState(StateStack& stack, Context context): 
	State(stack, context), mCarousel(context.textures)
{
	Textures::ID currentBackground = getCurrentBackgroundOption();
	mBackground.setTexture(context.textures->get(currentBackground));
	sf::Color color = mBackground.getColor();
	color.a = 0;
	mBackground.setColor(sf::Color(100, 100, 100));

	std::vector <Textures::ID> thumbnail = getCurrentThumbnailOption();
	mCarousel.pushItem(Carousel::Item::AVL, 		context.textures->get(thumbnail[0]), 	[this](){ requestStackPop(); requestStackPush(States::InAppAVL); });
	mCarousel.pushItem(Carousel::Item::HEAP, 		context.textures->get(thumbnail[1]), 	[this](){ requestStackPop(); requestStackPush(States::InAppHeap); });
	mCarousel.pushItem(Carousel::Item::LINKED_LIST, context.textures->get(thumbnail[2]), 	[this](){ requestStackPop(); requestStackPush(States::InAppLinkedList); });
	mCarousel.pushItem(Carousel::Item::GRAPH, 		context.textures->get(thumbnail[3]), 	[this](){ requestStackPop(); requestStackPush(States::InAppGraph); });

	// setting button
	mSettingButton = std::make_shared<GUI::Button>(context.fonts->get(Fonts::UI), sf::Vector2f(SCREEN::Width - 80.f, 20.f), "", sf::Vector2f(50.f, 50.f), GUI::Button::ShapeType::Circle, GUI::Button::ContentType::Image);
	mSettingButton->setNormalColor(sf::Color(255, 255, 255, 50));
	mSettingButton->setSelectedColor(sf::Color(255, 255, 255, 100));
	mSettingButton->setSprite(sf::Sprite(context.textures->get(Textures::SettingIcon)));
	mSettingButton->setCallback([this](){ requestStackPop(); requestStackPush(States::Settings); });

	context.textures->get(Textures::Title).setSmooth(true);
	mTitle.setTexture(context.textures->get(Textures::Title));
	centerOrigin(mTitle);
	mTitle.setPosition(sf::Vector2f(SCREEN::Width / 2.f, 100));
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mBackground);
	mCarousel.draw(window);
	window.draw(*mSettingButton);
	window.draw(mTitle);
}

bool MenuState::update(sf::Time dt)
{
	mCarousel.update(dt);
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	mSettingButton->handleEvent(event);
	mCarousel.handleEvent(event);
	return true;
}