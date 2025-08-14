#pragma once
#include "Context.h"

struct ICommand 
{
    virtual ~ICommand() = default;
    virtual const char* name() const = 0;
    virtual const char* help() const = 0;
    virtual void execute(Context& ctx, const std::vector<std::string>& args) = 0;
};