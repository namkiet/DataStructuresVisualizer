#include <State/SettingsState.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <iostream>
#include <string> 
#include <sstream>
#include <iomanip>

SettingsState::SettingsState(StateStack& stack, Context context): 
	context(context),
	State(stack, context)
{

	// NodeSize.setString(std::to_string(int(VIZ::NODE::Radius)));
	NodeSize.setFont(context.fonts->get(Fonts::UI));
	NodeSize.setCharacterSize(20);
	centerOrigin(NodeSize);
	NodeSize.setFillColor(sf::Color::White);
	NodeSize.setOutlineThickness(1.0f);
	NodeSize.setPosition(SCREEN::Width * 0.4f, SCREEN::Height* 0.6f);


	// EdgeThickness.setString(std::to_string(int(VIZ::EDGE::Thickness)));
	EdgeThickness.setFont(context.fonts->get(Fonts::UI));
	EdgeThickness.setCharacterSize(20);
	centerOrigin(EdgeThickness);
	EdgeThickness.setFillColor(sf::Color::White);
	EdgeThickness.setOutlineThickness(1.0f);
	EdgeThickness.setPosition(SCREEN::Width * 0.4f, SCREEN::Height* 0.7f);

	
	updateStatDisplay();
	
	//background sprite
	centerOrigin(mBackgroundSprite);
	mBackgroundSprite.setPosition(SCREEN::Width / 2.f, SCREEN::Height * 0.3f);
	spriteResize(mBackgroundSprite, sf::Vector2f(400.f,225.f));

	mBackgroundText.setFont(context.fonts->get(Fonts::UI));
	mBackgroundText.setString("Background");
	mBackgroundText.setCharacterSize(40);
	centerOrigin(mBackgroundText);
	mBackgroundText.setPosition(SCREEN::Width / 2.f, SCREEN::Height * 0.1f);
	mBackgroundText.setFillColor(sf::Color::White);
	mBackgroundText.setOutlineThickness(2.f);



	initNodesAndEdge();
	sf::Sprite HomeSprite;
    HomeSprite.setTexture(context.textures->get(Textures::HomeIcon));
    mHomeButton = std::make_shared<GUI::Button>(context.fonts->get(Fonts::UI), sf::Vector2f(20.f,20.f), "", sf::Vector2f(50.f,50.f), GUI::Button::ShapeType::Circle, GUI::Button::ContentType::Image);
    mHomeButton->setSprite(HomeSprite);
	mHomeButton->setCallback([this](){
		requestStackPop();
		requestStackPush(States::Menu);
	});


	mNodeSizeText.setFont(context.fonts->get(Fonts::UI));
	mNodeSizeText.setString("NODE SIZE");
	mNodeSizeText.setCharacterSize(20);
	centerOrigin(mNodeSizeText);
	mNodeSizeText.setPosition(SCREEN::Width * 0.2f, SCREEN::Height * 0.6f);
	mNodeSizeText.setFillColor(sf::Color::White);
	mNodeSizeText.setOutlineThickness(1.5f);
	
	mEdgeThicknessText.setFont(context.fonts->get(Fonts::UI));
	mEdgeThicknessText.setString("EDGE THICKNESS");
	mEdgeThicknessText.setCharacterSize(20);
	centerOrigin(mEdgeThicknessText);
	mEdgeThicknessText.setPosition(SCREEN::Width * 0.2f, SCREEN::Height * 0.7f);
	mEdgeThicknessText.setFillColor(sf::Color::White);
	mEdgeThicknessText.setOutlineThickness(1.5f);


	// move left background
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
		updateStatDisplay();
	});
	
	// move right background
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
		updateStatDisplay();
	});
	
	// background text option
	mBackgroundTextOption.setFont(context.fonts->get(Fonts::UI));
	mBackgroundTextOption.setCharacterSize(20);
	centerOrigin(mBackgroundTextOption);
	mBackgroundTextOption.setFillColor(sf::Color::White);
	mBackgroundTextOption.setOutlineThickness(1.5f);
	mBackgroundTextOption.setPosition(SCREEN::Width / 2.f, SCREEN::Height / 2.f);

	// minus and plus sign for node size
	DecreaseNodeSize = std::make_shared<GUI::Button>(
		context.fonts->get(Fonts::UI),
		sf::Vector2f(SCREEN::Width * 0.35f - 25.f, SCREEN::Height * 0.6f - 25.f),
		"",
		sf::Vector2f(50.f, 50.f),
		GUI::Button::ShapeType::Circle,
		GUI::Button::ContentType::Image
	);
	

	sf::Sprite DecreaseSprite;
	DecreaseSprite.setTexture(context.textures->get(Textures::ID::Minus));
	centerOrigin(DecreaseSprite);
	DecreaseNodeSize->setSprite(DecreaseSprite);
	
	DecreaseNodeSize->setToggle(false);
	DecreaseNodeSize->setCallback([this]() {
		if (VIZ::NODE::Radius > VIZ::NODE::MinRadius) { // ví dụ điều kiện tối thiểu
			float newSize = VIZ::NODE::Radius - 2.f;
			VIZ::NODE::Thickness = newSize * (VIZ::NODE::Thickness / VIZ::NODE::Radius);
			VIZ::NODE::Radius = newSize;
			updateStatDisplay();
	
			for (auto& node : nodes) delete node;
			for (auto& edge : edges) delete edge;
			initNodesAndEdge();
		}
	});

	IncreaseNodeSize = std::make_shared<GUI::Button>(
		context.fonts->get(Fonts::UI),
		sf::Vector2f(SCREEN::Width * 0.45f - 25.f, SCREEN::Height * 0.6f - 25.f), // đối xứng với nút trừ
		"",
		sf::Vector2f(50.f, 50.f),
		GUI::Button::ShapeType::Circle,
		GUI::Button::ContentType::Image
	);
	
	sf::Sprite IncreaseSprite;
	IncreaseSprite.setTexture(context.textures->get(Textures::ID::Plus));
	centerOrigin(IncreaseSprite);
	IncreaseNodeSize->setSprite(IncreaseSprite);
	
	IncreaseNodeSize->setToggle(false);
	IncreaseNodeSize->setCallback([this]() {
		if (VIZ::NODE::Radius < VIZ::NODE::MaxRadius) { // ví dụ giới hạn tối đa
			float newSize = VIZ::NODE::Radius + 2.f;
			VIZ::NODE::Thickness = newSize * (VIZ::NODE::Thickness / VIZ::NODE::Radius);
			VIZ::NODE::Radius = newSize;
			updateStatDisplay();
	
			for (auto& node : nodes) delete node;
			for (auto& edge : edges) delete edge;
			initNodesAndEdge();
		}
	});

	
	// plus and minus sign for thickness of edge
	DecreaseEdgeThickness = std::make_shared<GUI::Button>(
		context.fonts->get(Fonts::UI),
		sf::Vector2f(SCREEN::Width * 0.35f - 25.f, SCREEN::Height * 0.7f - 25.f), // vị trí y tương ứng EdgeText
		"",
		sf::Vector2f(50.f, 50.f),
		GUI::Button::ShapeType::Circle,
		GUI::Button::ContentType::Image
	);
	
	sf::Sprite DecreaseEdgeSprite;
	DecreaseEdgeSprite.setTexture(context.textures->get(Textures::ID::Minus));
	centerOrigin(DecreaseEdgeSprite);
	DecreaseEdgeThickness->setSprite(DecreaseEdgeSprite);
	
	DecreaseEdgeThickness->setToggle(false);
	DecreaseEdgeThickness->setCallback([this]() {
		if (VIZ::EDGE::Thickness > VIZ::EDGE::MinThickness) {
			VIZ::EDGE::Thickness -= VIZ::EDGE::ChangeStep;
			updateStatDisplay();
	
			for (auto& node : nodes) delete node;
			for (auto& edge : edges) delete edge;
			initNodesAndEdge();
		}
	});

	IncreaseEdgeThickness = std::make_shared<GUI::Button>(
		context.fonts->get(Fonts::UI),
		sf::Vector2f(SCREEN::Width * 0.45f - 25.f, SCREEN::Height * 0.7f - 25.f),
		"",
		sf::Vector2f(50.f, 50.f),
		GUI::Button::ShapeType::Circle,
		GUI::Button::ContentType::Image
	);
	
	sf::Sprite IncreaseEdgeSprite;
	IncreaseEdgeSprite.setTexture(context.textures->get(Textures::ID::Plus));
	centerOrigin(IncreaseEdgeSprite);
	IncreaseEdgeThickness->setSprite(IncreaseEdgeSprite);
	
	IncreaseEdgeThickness->setToggle(false);
	IncreaseEdgeThickness->setCallback([this]() {
		if (VIZ::EDGE::Thickness < VIZ::EDGE::MaxThickness) {
			VIZ::EDGE::Thickness += VIZ::EDGE::ChangeStep;
			updateStatDisplay();
	
			for (auto& node : nodes) delete node;
			for (auto& edge : edges) delete edge;
			initNodesAndEdge();
		}
	});
	
}

void SettingsState::updateStatDisplay(){
	// base on the actual info, update the stat in setting
	
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

	NodeSize.setString(std::to_string(int(VIZ::NODE::Radius)));
	centerOrigin(NodeSize);
	
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(1) << VIZ::EDGE::Thickness;
	EdgeThickness.setString(oss.str());
	centerOrigin(EdgeThickness);

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
	window.draw(mEdgeThicknessText);
	window.draw(mNodeSizeText);

	window.draw(mBackgroundSprite);
	window.draw(mBackgroundTextOption);
	window.draw(*BgNavigateLeft);
	window.draw(*BgNavigateRight);
	window.draw(NodeSize);
	window.draw(EdgeThickness);

	if(VIZ::NODE::Radius < VIZ::NODE::MaxRadius) window.draw(*IncreaseNodeSize);
	if(VIZ::NODE::Radius > VIZ::NODE::MinRadius) window.draw(*DecreaseNodeSize);

	if(VIZ::EDGE::Thickness < VIZ::EDGE::MaxThickness) window.draw(*IncreaseEdgeThickness);
	if(VIZ::EDGE::Thickness > VIZ::EDGE::MinThickness) window.draw(*DecreaseEdgeThickness);
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
	IncreaseNodeSize->handleEvent(event);
	DecreaseNodeSize->handleEvent(event);

	IncreaseEdgeThickness->handleEvent(event);
	DecreaseEdgeThickness->handleEvent(event);
	return true;
}