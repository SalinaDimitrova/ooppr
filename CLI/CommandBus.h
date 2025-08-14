#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Tokenize.h"
#include "ICommand.h"
#include "Context.h"

class CommandBus {
public:
    CommandBus() = default;

    void registerCommand(std::unique_ptr<ICommand> cmd) 
    {
        cmds_.push_back(std::move(cmd));
    }

    template <typename T, typename... Args>
    void emplace(Args&&... args) 
    {
        std::unique_ptr<ICommand> c(new T(std::forward<Args>(args)...));
        cmds_.push_back(std::move(c));
    }

    bool dispatch(Context& ctx, const std::string& line) 
    {
        std::vector<std::string> tokens = tokenize(line);
        if (tokens.empty()) 
            return false;

        const std::string cmdName = tokens[0];
        std::vector<std::string> args;
        if (tokens.size() > 1) 
        {
            args.assign(tokens.begin() + 1, tokens.end());
        }

        for (std::size_t i = 0; i < cmds_.size(); ++i)
         {
            if (cmdName == cmds_[i]->name()) 
            {
                cmds_[i]->execute(ctx, args);
                return true;
            }
        }
        std::cout << "Unknown command.\n";
        return false;
    }

    void listCommands(Context& ctx) const 
    {
        for (std::size_t i = 0; i < cmds_.size(); ++i) 
        {
            ctx.out << cmds_[i]->name() << " - " << cmds_[i]->help() << "\n";
        }
    }

private:
    std::vector<std::unique_ptr<ICommand>> cmds_;
};
