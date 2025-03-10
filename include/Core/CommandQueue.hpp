#pragma once
#include <Core/Command.hpp>
#include <queue>

class CommandQueue
{
public:
    void                        push(Command command);
    Command                     pop();
    bool                        isEmpty() const;

    void                        undo();
    void                        redo();

private:
    std::queue<Command>         mQueue;
    std::stack<Command>         mUndoStack;
    std::stack<Command>         mRedoStack;
};