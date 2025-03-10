#include <Core/User.hpp>
#include <SceneNode/TreeNode.hpp>
#include <SceneNode/TreeNode.hpp>
#include <iostream>
#include "Core/Button.hpp"
#include "Core/CommandQueue.hpp"


User::User(): mActionBinding()
{

}

void User::handleButton(std::shared_ptr<Button> button, CommandQueue& commands){
    User::Action action = static_cast<User::Action>(button->getAction());
    auto it = mActionBinding.find(action);  
    if (it != mActionBinding.end())
    {
        commands.push(it->second);
    }
}