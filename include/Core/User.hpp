#pragma once
#include <SFML/Graphics.hpp>
#include <Core/CommandQueue.hpp>
#include <Core/Command.hpp>
#include <memory>
class Button;

class User
{
public:
    enum class Action
    {
        None,
        Insert,
        Delete,
    };

    User();

    void handleEvent(const sf::Event& event, CommandQueue& commands);
    void handleButton(std::shared_ptr<Button> button, CommandQueue& commands);

private:

    void initializeActions();

    std::map<Action, Command> mActionBinding;
};
