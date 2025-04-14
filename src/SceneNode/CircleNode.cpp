#include <SceneNode/CircleNode.hpp>
#include <Core/Utility.hpp>
#include <Core/Variables.hpp>

CircleNode::CircleNode(const CircleNode &other)
    : sf::Transformable(other), sf::Drawable(other),
      mValue(other.mValue), mTargetPosition(other.mTargetPosition),
      mShape(other.mShape), mText(other.mText), mNote(other.mNote),
      mFont(other.mFont), mTextSize(other.mTextSize), mOpacity(other.mOpacity)
{
    // Cập nhật font cho text
    mText.setFont(mFont);
    mNote.setFont(mFont);
}

CircleNode::CircleNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor):
    mValue(value), mOpacity(1.f)
{
    mShape.setRadius(radius);
    mShape.setOrigin(sf::Vector2f(radius, radius));
    mShape.setFillColor(VIZ::NODE::FillColor);
    mShape.setOutlineColor(VIZ::NODE::OutlineColor);
    mShape.setOutlineThickness(VIZ::NODE::Thickness);
    mShape.setPointCount(10000);

    mFont.loadFromFile("assets/fonts/jetbrains.ttf");

    mTextSize = radius * 0.9;
    
    mText.setFont(mFont);
    mText.setColor(VIZ::TextColor);
    mText.setPosition(mShape.getPosition());
    updateText();

    mNote.setFont(mFont);
    mNote.setColor(VIZ::TextColor);
    mNote.setPosition(mShape.getPosition() + sf::Vector2f(mTextSize, radius + 10));
    mNote.setCharacterSize(mTextSize);
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
    if (opacity > 1) opacity = 1;

    mOpacity = opacity;
    int alpha = int(255.f * mOpacity);

    sf::Color newFillColor = mShape.getFillColor();
    if (VIZ::NODE::FillColor != sf::Color::Transparent)
    {
        newFillColor.a = alpha;
        mShape.setFillColor(newFillColor);
    }

    sf::Color newOutlineColor = mShape.getOutlineColor();
    newOutlineColor.a = alpha;
    mShape.setOutlineColor(newOutlineColor);

    sf::Color newTextColor = mText.getFillColor();
    newTextColor.a = alpha;
    mText.setFillColor(newTextColor);

    sf::Color newNoteColor = mNote.getFillColor();
    newNoteColor.a = alpha;
    mNote.setFillColor(newNoteColor);
}

float CircleNode::getOpacity()
{
    return mOpacity;
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

void CircleNode::setSize(float size)
{
    mShape.setOutlineThickness(mShape.getOutlineThickness() / mShape.getRadius() * size);
    mTextSize = mTextSize / mShape.getRadius() * size;
    updateText();
    mShape.setRadius(size);
    mShape.setOrigin(sf::Vector2f(size, size));
    mText.setPosition(mShape.getPosition());
}