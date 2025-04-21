#include <State/AboutState.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/Utility.hpp>

AboutState::AboutState(StateStack& stack, Context context)
: State(stack, context), context(context)
{

    mAboutTitle.setFont(context.fonts->get(Fonts::Main));
    mAboutTitle.setString("About");
    mAboutTitle.setCharacterSize(50); 
    mAboutTitle.setFillColor(sf::Color::White);
    centerOrigin(mAboutTitle);
    mAboutTitle.setPosition(SCREEN::Width * 0.5f, 50.f);

    // Background
    Textures::ID currentBackground = getCurrentBackgroundOption();
	mBackground.setTexture(context.textures->get(currentBackground));

    // Font
    const sf::Font& font = context.fonts->get(Fonts::Main);

    // Project Description
    mProjectDescription.setString(
        "This is the CS163 Group Lab Project:\n"
        "Data Structure Visualizer Project.\n"
        "It helps users understand how several\n"
        "fundamental data structures operate\n"
        "through interactive animations."
    );
    mProjectDescription.setCharacterSize(20);
    mProjectDescription.setFont(font);
    mProjectDescription.setFillColor(sf::Color::White);
    centerOrigin(mProjectDescription);
    mProjectDescription.setPosition(SCREEN::Width * 0.5f , SCREEN::Height * 0.3f);

    sf::FloatRect bounds = mProjectDescription.getGlobalBounds();
    float padding = 20.f;
    mDescriptionBox.setSize(sf::Vector2f(bounds.width + padding * 2, bounds.height + padding * 2));
    mDescriptionBox.setFillColor(sf::Color(0, 0, 0, 100));
    mDescriptionBox.setOutlineColor(sf::Color::White);
    mDescriptionBox.setOutlineThickness(2.f);
    mDescriptionBox.setOrigin(mDescriptionBox.getSize() / 2.f);
    mDescriptionBox.setPosition(mProjectDescription.getPosition());

    std::vector<std::string> memberStrings = {
        "Trinh Vo Nam Kiet - 24125013",
        "Nguyen Huu Phuoc - 24125018",
        "Le Quoc Vi - 24125085"
    };
    
    float baseY = SCREEN::Height * 0.55f;
    float spacing = 35.f;
    
    for (size_t i = 0; i < memberStrings.size(); i++) {
        sf::Text member;
        member.setFont(font);
        member.setString(memberStrings[i]);
        member.setCharacterSize(20);
        member.setFillColor(sf::Color::White);
        member.setPosition(SCREEN::Width * 0.4f, baseY + i * spacing);
        mTeamTexts.push_back(member);
    }

    mBullet1.setRadius(5.f);
    mBullet1.setFillColor(sf::Color::White);
    mBullet1.setOrigin(5.f, 5.f);
    mBullet1.setPosition(mTeamTexts[0].getPosition().x - 30.f, mTeamTexts[0].getPosition().y + 13.f);

    mBullet2.setRadius(5.f);
    mBullet2.setFillColor(sf::Color::White);
    mBullet2.setOrigin(5.f, 5.f);
    mBullet2.setPosition(mTeamTexts[1].getPosition().x - 30.f, mTeamTexts[1].getPosition().y + 13.f);

    mBullet3.setRadius(5.f);
    mBullet3.setFillColor(sf::Color::White);
    mBullet3.setOrigin(5.f, 5.f);
    mBullet3.setPosition(mTeamTexts[2].getPosition().x - 30.f, mTeamTexts[2].getPosition().y + 13.f);

    // Home Button
    sf::Sprite HomeSprite;
    HomeSprite.setTexture(context.textures->get(Textures::HomeIcon));
    mHomeButton = std::make_shared<GUI::Button>(context.fonts->get(Fonts::UI), sf::Vector2f(20.f,20.f), "", sf::Vector2f(50.f,50.f), GUI::Button::ShapeType::Circle, GUI::Button::ContentType::Image);
    mHomeButton->setSprite(HomeSprite);
	mHomeButton->setCallback([this](){
		requestStackPop();
		requestStackPush(States::Menu);
	});

}

bool AboutState::update(sf::Time dt)
{
    return true;
}
bool AboutState::handleEvent(const sf::Event& event)
{
    mHomeButton->handleEvent(event);
    return true;
}
void AboutState::draw()
{
    sf::RenderWindow& window = *context.window;
    window.draw(mBackground);
    window.draw(mAboutTitle);
    window.draw(mDescriptionBox);
    window.draw(mProjectDescription);
    for (const auto& member : mTeamTexts) {
        window.draw(member);
    }
    window.draw(mBullet1);
    window.draw(mBullet2);
    window.draw(mBullet3);
    window.draw(*mHomeButton);
}