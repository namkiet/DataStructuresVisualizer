#include <SFML/System/Time.hpp>
#include <queue>
#include <stack>
#include <functional>

class ActionQueue 
{
public:
    using ActionFunc = std::function<bool(sf::Time)>;

    void empty();

    void pushUndo(ActionFunc action) {
        // undoStack.push(std::move(action));
        // redoStack = std::stack<ActionFunc>(); // Xóa redo khi có hành động mới
    }

    void pushRedo(ActionFunc action) {
        // redoStack.push(std::move(action));
    }

    void undo() {
        // if (undoStack.empty() || !queue.empty()) return;
        // auto action = std::move(undoStack.top());
        // undoStack.pop();
        // redoStack.push(action);
        // pushAction(action);
    }

    void redo() {
        // if (redoStack.empty()) return;
        // auto action = std::move(redoStack.top());
        // redoStack.pop();
        // undoStack.push(action);
        // return action(dt);
    }

    bool canUndo() const { return !undoStack.empty(); }
    bool canRedo() const { return !redoStack.empty(); }

private:
    std::stack<ActionFunc> undoStack;
    std::stack<ActionFunc> redoStack;
    std::deque<std::vector<ActionFunc>> queue;

public:
    void pushAction(ActionFunc action);
    void update(sf::Time dt);
    void createNewBatch();
    bool empty() const;
};
