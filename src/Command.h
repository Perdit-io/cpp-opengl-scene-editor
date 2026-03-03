#pragma once

class Command {
public:
    virtual ~Command() = default;
    virtual void Execute() = 0; // Re-apply the change
    virtual void Undo() = 0;    // Revert the change
};
