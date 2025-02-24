#include <CommandQueue.hpp>

void CommandQueue::push(Command command)
{
    mQueue.push(command);
}

Command CommandQueue::pop()
{
    Command command = mQueue.front();
    mQueue.pop();
    return command;
}


bool CommandQueue::push()
{
    return mQueue.isEmpty();
}