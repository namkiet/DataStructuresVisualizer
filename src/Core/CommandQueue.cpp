#include <Core/CommandQueue.hpp>

void CommandQueue::push(Command command)
{
    mQueue.push(command);

    mUndoStack
    mRedoStack.clear();
}

Command CommandQueue::pop()
{
    Command command = mQueue.front();
    mQueue.pop();

    mUndoStack.push(command);
    return command;
}


bool CommandQueue::isEmpty() const
{
    return mQueue.empty();
}

void CommandQueue::undo()
{
    if (mUndoStack.empty()) return;
    Command command = mUndoStack.top();
    mUndoStack.pop();
    
    mQueue.push(command);
    mRedoStack.push(command);
}