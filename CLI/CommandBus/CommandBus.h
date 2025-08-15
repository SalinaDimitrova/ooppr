#pragma once
#include <vector>
#include <string>

class ICommand;
class Context;

class CommandBus {
public:
    CommandBus();
    ~CommandBus();

    CommandBus(const CommandBus&) = delete;
    CommandBus& operator=(const CommandBus&) = delete;

    CommandBus(CommandBus&& other) noexcept;
    CommandBus& operator=(CommandBus&& other) noexcept;

    void registerCommand(ICommand* cmd);

    template <typename T, typename... Args>
    void emplace(Args&&... args) {
        
        ICommand* c = new T(static_cast<Args&&>(args)...); 
        cmds.push_back(c);
    }

    bool dispatch(Context& ctx, const std::string& line);
    void listCommands(Context& ctx) const;

private:
    void clear();

    std::vector<ICommand*> cmds;
};
