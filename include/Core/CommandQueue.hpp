#pragma once
#include <Core/Command.hpp>
#include <queue>

class CommandQueue
{
public:
    void                push(Command command);
    Command             pop();
    bool                isEmpty() const;

private:
    std::queue<Command> mQueue;
};