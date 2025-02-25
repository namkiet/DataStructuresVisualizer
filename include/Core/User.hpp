#pragma once
#include <SFML/Graphics.hpp>
#include <Core/CommandQueue.hpp>
#include <Core/Command.hpp>

class User
{
public:
    void        handleEvent(sf::Event event, CommandQueue& commands);
};