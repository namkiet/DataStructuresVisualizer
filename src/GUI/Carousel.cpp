#include <GUI/Carousel.hpp>

void Carousel::pushItem(Item::ItemType type, std::string imageName, std::function<void()> callback)
{
    items.push_back(new Item(type, imageName));
    items.back()->setCallback(callback);

    for (int i = 0; i < items.size(); i++)
    {
        items[i]->mInitialPosition = sf::Vector2f(SCREEN::Width / 2 + i * 350, SCREEN::Height / 2);
        items[i]->setDotPosition(sf::Vector2f(SCREEN::Width / 2 + i * 20 - 20, SCREEN::Height / 2 + 250));
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
        float opacity = currentItem->getOpacity();
        if (currentItem->mIsSelected)
        {
            scale += (selectedScale - scale) * 5 * dt.asSeconds();
            opacity += (1.f - opacity) * 5 * dt.asSeconds();
        }
        else
        {
            scale += (defaultScale - scale) * 5 * dt.asSeconds();
            opacity += (0.5f - opacity) * 5 * dt.asSeconds();
        }

        currentItem->setScale(scale);
        currentItem->setOffset(sf::Vector2f(currentOffset, 0));
        currentItem->setOpacity(opacity);
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