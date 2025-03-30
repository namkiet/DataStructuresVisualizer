#include <GUI/Carousel.hpp>
#include <iostream>

void Carousel::pushItem(Item::ItemType type, std::string imageName, GUI::Button::Callback callback)
{
    items.push_back(new Item(type, imageName));
    items.back()->setCallback(callback);

    for (int i = 0; i < items.size(); i++)
    {
        items[i]->mInitialPosition = sf::Vector2f(SCREEN::Width / 2 + i * 350, SCREEN::Height / 2);
        items[i]->setDotPosition(sf::Vector2f(SCREEN::Width / 2 + i * 20 - 20, 600));
        items[i]->setScale(defaultScale);
    }

    currentIndex = 0;
    items[0]->mIsSelected = true;
}

void Carousel::next()
{
    items[currentIndex]->mIsSelected = false;
    currentIndex = (currentIndex + 1) % items.size();
    targetOffset = -350 * currentIndex;
    items[currentIndex]->mIsSelected = true;
}

void Carousel::prev()
{
    items[currentIndex]->mIsSelected = false;
    currentIndex = (currentIndex - 1 + items.size()) % items.size();
    targetOffset = -350 * currentIndex;
    items[currentIndex]->mIsSelected = true;
}

void Carousel::update(sf::Time dt)
{
    currentOffset += (targetOffset - currentOffset) * 5 * dt.asSeconds();

    for (auto &currentItem: items)
    {
        float scale = currentItem->getScale();
        float opacity;
        if (currentItem->mIsSelected)
        {
            scale += (selectedScale - currentItem->getScale()) * 5 * dt.asSeconds();
            opacity = 1.f;
        }
        else
        {
            scale += (defaultScale - currentItem->getScale()) * 5 * dt.asSeconds();
            opacity = 0.5f;
        }

        currentItem->setScale(scale);
        currentItem->setOffset(sf::Vector2f(currentOffset, 0));
        currentItem->setOpacity(opacity);
        currentItem->setDotOpacity(opacity);
    }
}

void Carousel::checkClick(sf::Vector2f mousePos)
{
    for (int i = 0; i < items.size(); i++)
    {
        if (i == currentIndex && items[i]->isClicked(mousePos))
        {
            // std::cout << "Clicked on: " << items[i]->name << std::endl;
        }
    }
}

void Carousel::handleEvent(sf::Event event)
{  
    if (event.type == sf::Event::MouseButtonPressed) 
    {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        if (items[currentIndex]->isClicked(mousePos))
            items[currentIndex]->mCallback();
    }
}

void Carousel::draw(sf::RenderWindow& window)
{
    // Draw unselected items first so that the selected one is on top
    for (auto &currentItem: items)
    {
        if (currentItem->mIsSelected) 
            continue;    

        currentItem->draw(window);
    }

    items[currentIndex]->draw(window);
}