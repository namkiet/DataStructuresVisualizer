#include <State/SettingsState.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <iostream>

SettingsState::SettingsState(StateStack& stack, Context context): 
	context(context),
	State(stack, context)
{
	initNodesAndEdge();
	sf::Sprite HomeSprite;
    HomeSprite.setTexture(context.textures->get(Textures::HomeIcon));
    mHomeButton = std::make_shared<GUI::Button>(context.fonts->get(Fonts::UI), sf::Vector2f(20.f,20.f), "", sf::Vector2f(50.f,50.f), GUI::Button::ShapeType::Circle, GUI::Button::ContentType::Image);
    mHomeButton->setSprite(HomeSprite);
	mHomeButton->setCallback([this](){
		requestStackPop();
		requestStackPush(States::Menu);
	});

	mBackgroundText.setFont(context.fonts->get(Fonts::UI));
	mBackgroundText.setString("Background");
	mBackgroundText.setCharacterSize(40);
	centerOrigin(mBackgroundText);
	mBackgroundText.setPosition(SCREEN::Width / 2.f, SCREEN::Height * 0.1f);
	mBackgroundText.setFillColor(sf::Color::White);
	mBackgroundText.setOutlineThickness(2.f);


	resetBackgroundOption();
	centerOrigin(mBackgroundSprite);
	mBackgroundSprite.setPosition(SCREEN::Width / 2.f, SCREEN::Height * 0.3f);
	spriteResize(mBackgroundSprite, sf::Vector2f(400.f,225.f));

	BgNavigateLeft = std::make_shared<GUI::Button>(context.fonts->get(Fonts::UI), mBackgroundSprite.getPosition(), "", sf::Vector2f(100.f,100.f), GUI::Button::ShapeType::Circle, GUI::Button::ContentType::Image);
	sf::Sprite LeftSprite;
    LeftSprite.setTexture(context.textures->get(Textures::NavigateLeft));
    BgNavigateLeft->setSprite(LeftSprite);
	centerOrigin(LeftSprite);
	BgNavigateLeft->setToggle(false);
	BgNavigateLeft->setPosition(mBackgroundSprite.getPosition() - sf::Vector2f(50.f,50.f) - sf::Vector2f(270.f,0.f));
	BgNavigateLeft->setCallback([this]() {
		int index = static_cast<int>(THEME::mTheme);
		index = (index - 1 + 3) % 3;
		THEME::mTheme = static_cast<THEME::Theme>(index);
		resetBackgroundOption();
	});

	BgNavigateRight = std::make_shared<GUI::Button>(
		context.fonts->get(Fonts::UI),
		mBackgroundSprite.getPosition(),
		"",
		sf::Vector2f(100.f, 100.f),
		GUI::Button::ShapeType::Circle,
		GUI::Button::ContentType::Image
	);
	sf::Sprite RightSprite;
	RightSprite.setTexture(context.textures->get(Textures::NavigateRight));
	centerOrigin(RightSprite);
	BgNavigateRight->setToggle(false);
	BgNavigateRight->setSprite(RightSprite);
	BgNavigateRight->setPosition(mBackgroundSprite.getPosition() - sf::Vector2f(50.f,50.f) + sf::Vector2f(270.f, 0.f));
	BgNavigateRight->setCallback([this]() {
		int index = static_cast<int>(THEME::mTheme);
		index = (index + 1) % 3;
		THEME::mTheme = static_cast<THEME::Theme>(index);
		resetBackgroundOption();
	});
	

	mBackgroundTextOption.setFont(context.fonts->get(Fonts::UI));
	mBackgroundTextOption.setCharacterSize(20);
	centerOrigin(mBackgroundTextOption);
	mBackgroundTextOption.setFillColor(sf::Color::White);
	mBackgroundTextOption.setOutlineThickness(1.5f);
	mBackgroundTextOption.setPosition(SCREEN::Width / 2.f, SCREEN::Height / 2.f);

}

void SettingsState::resetBackgroundOption(){
	
	if(THEME::mTheme == THEME::Theme::DesertNight)
	{
		mBackgroundSprite.setTexture(context.textures->get(Textures::AppBackground1));
		centerOrigin(mBackgroundText);
		mBackgroundTextOption.setString("Desert Night");
		centerOrigin(mBackgroundTextOption);
	}
	else if (THEME::mTheme == THEME::Theme::Sunrise)
	{
		mBackgroundSprite.setTexture(context.textures->get(Textures::AppBackground2));
		centerOrigin(mBackgroundText);
		mBackgroundTextOption.setString("Sunrise");
		centerOrigin(mBackgroundTextOption);
	}
	else if (THEME::mTheme == THEME::Theme::Forest)
	{
		mBackgroundSprite.setTexture(context.textures->get(Textures::AppBackground3));
		mBackgroundTextOption.setString("Forest");
		centerOrigin(mBackgroundTextOption);
	}

}
void SettingsState::initNodesAndEdge()
{
	nodes.resize(3); 
	edges.resize(2);
    nodes[0] = new CircleNode(1, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);
    nodes[1] = new CircleNode(2, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);
	nodes[2] = new CircleNode(3, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);

	nodes[0]->setPosition(sf::Vector2f(1200, 500));
	nodes[1]->setPosition(sf::Vector2f(1100, 700));
	nodes[2]->setPosition(sf::Vector2f(1300, 700));
	edges[0] = new Edge(VIZ::EDGE::Color, nodes[0], nodes[1], false, VIZ::EDGE::Thickness);
	edges[1] = new Edge(VIZ::EDGE::Color, nodes[0], nodes[2], false, VIZ::EDGE::Thickness);
}

void SettingsState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
 
	for(CircleNode* node : nodes)
	{
		window.draw(*node);
	}
	for(Edge* edge : edges)
	{
		window.draw(*edge);
	}
	window.draw(*mHomeButton);

	window.draw(mBackgroundText);
	window.draw(mBackgroundSprite);
	window.draw(mBackgroundTextOption);
	window.draw(*BgNavigateLeft);
	window.draw(*BgNavigateRight);
}

bool SettingsState::update(sf::Time dt)
{
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{

	// home button first
	mHomeButton->handleEvent(event);
	BgNavigateLeft->handleEvent(event);
	BgNavigateRight->handleEvent(event);

	if (event.type == sf::Event::KeyPressed)
	{
        if (event.key.code == sf::Keyboard::B)
        {
			if (VIZ::NODE::Radius < 36.f)
			{
				float newSize = VIZ::NODE::Radius + 2.f;
				VIZ::NODE::Thickness = newSize * (VIZ::NODE::Thickness / VIZ::NODE::Radius);
				VIZ::NODE::Radius = newSize;
			}
        }

		if (event.key.code == sf::Keyboard::C)
		{
			if (VIZ::EDGE::Thickness < 7.f)
			{
				VIZ::EDGE::Thickness += 1.f;
			}
		}
	
		for(auto& node : nodes)
		{
			delete node;
		}
		for(auto& edge : edges)
		{
			delete edge;
		}
		initNodesAndEdge();
	}
	return true;
}