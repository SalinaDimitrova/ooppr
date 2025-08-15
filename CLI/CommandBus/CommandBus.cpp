#include "CommandBus.h"
#include "../ICommand.h"   
#include "../Context.h"
#include "../Tokenize.h"
#include <iostream>

CommandBus::CommandBus() = default;

CommandBus::~CommandBus() {
    clear();
}

CommandBus::CommandBus(CommandBus&& other) noexcept 
{
    cmds.swap(other.cmds);
}


CommandBus& CommandBus::operator=(CommandBus&& other) noexcept 
{
    if (this != &other) {
        clear();
        cmds.swap(other.cmds);
    }
    return *this;
}

void CommandBus::registerCommand(ICommand* cmd) 
{
    cmds.push_back(cmd);
}

bool CommandBus::dispatch(Context& ctx, const std::string& line) 
{
    std::vector<std::string> tokens = tokenize(line);
    if (tokens.empty())
        return false;

    const std::string& cmdName = tokens[0];
    std::vector<std::string> args;
    if (tokens.size() > 1)
        args.assign(tokens.begin() + 1, tokens.end());

    for (std::size_t i = 0; i < cmds.size(); ++i) 
    {
        if (cmds[i] && cmdName == cmds[i]->name()) 
        {
            cmds[i]->execute(ctx, args);
            return true;
        }
    }
    std::cout << "Unknown command.\n";
    return false;
}

void CommandBus::listCommands(Context& ctx) const 
{
    for (std::size_t i = 0; i < cmds.size(); ++i) 
    {
        if (cmds[i])
            ctx.out << cmds[i]->name() << " - " << cmds[i]->help() << "\n";
    }
}

void CommandBus::clear() 
{
    for (std::size_t i = 0; i < cmds.size(); ++i)
        delete cmds[i];
        
    cmds.clear();
}
