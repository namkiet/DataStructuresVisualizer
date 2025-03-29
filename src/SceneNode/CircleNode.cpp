#include <SceneNode/CircleNode.hpp>
#include <Core/Utility.hpp>

CircleNode::CircleNode(const CircleNode &other)
    : sf::Transformable(other), sf::Drawable(other),
      mValue(other.mValue), mTargetPostion(other.mTargetPostion),
      mShape(other.mShape), mText(other.mText), mNote(other.mNote),
      mFont(other.mFont), mTextSize(other.mTextSize) 
{
    // Cập nhật font cho text
    mText.setFont(mFont);
    mNote.setFont(mFont);
}

CircleNode::CircleNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor):
    mValue(value)
{
    mShape.setRadius(radius);
    mShape.setOrigin(sf::Vector2f(radius, radius));
    mShape.setFillColor(fillColor);
    mShape.setOutlineColor(outlineColor);
    mShape.setOutlineThickness(2);
    mShape.setPointCount(10000);

    mFont.loadFromFile("assets/fonts/jetbrains.ttf");
    
    mTextSize = radius * 0.75
    mText.setFont(mFont);
    mText.setColor(sf::Color::Black);
    mText.setPosition(mShape.getPosition());
    updateText();

    mNote.setFont(mFont);
    mNote.setColor(sf::Color::Black);
    mNote.setPosition(mShape.getPosition() + sf::Vector2f(0, radius + 10));
    mNote.setCharacterSize(mTextSize * 0.8f);
    setNote("");
}

void CircleNode::update(sf::Time dt) 
{
}

void CircleNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mShape, states);
    target.draw(mText, states);
    target.draw(mNote, states);
}

sf::Color CircleNode::getFillColor()
{
    return mShape.getFillColor();
}

void CircleNode::setFillColor(sf::Color color)
{
    mShape.setFillColor(color);
}

sf::Color CircleNode::getOutlineColor()
{
    return mShape.getOutlineColor();
}

void CircleNode::setOutlineColor(sf::Color color)
{
    mShape.setOutlineColor(color);
}


void CircleNode::setOpacity(float opacity)
{
    if (opacity > 1) return;
    int alpha = int(255 * opacity);
    sf::Color newFillColor = mShape.getFillColor();
    newFillColor.a = alpha;
    sf::Color newOutlineColor = mShape.getOutlineColor();
    newOutlineColor.a = alpha;
    sf::Color newTextColor = mText.getFillColor();
    newTextColor.a = alpha;
    
    mShape.setFillColor(newFillColor);
    mShape.setOutlineColor(newOutlineColor);
    mText.setFillColor(newTextColor);
}

float CircleNode::getOpacity()
{
    return mShape.getFillColor().a / 255.f;
}

float CircleNode::getRadius()
{
    return mShape.getRadius() + mShape.getOutlineThickness();
}

void CircleNode::setValue(int value)
{
    mValue = value;
    updateText();
}

float CircleNode::getTextSize()
{
    return mText.getCharacterSize();
}

void CircleNode::setTextSize(float textSize)
{
    mTextSize = textSize;
    updateText();
}

void CircleNode::updateText()
{
    mText.setCharacterSize(mTextSize);
    mText.setString(std::to_string(mValue));
    centerOrigin(mText);
}

void CircleNode::setNote(std::string note)
{
    mNote.setString(note);
    centerOrigin(mNote);
}