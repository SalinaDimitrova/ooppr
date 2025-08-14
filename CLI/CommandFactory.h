#pragma once
#include "CommandBus.h"
#include "Commands/Commands.h"

struct CommandFactory {
    static void registerDefaultCommands(CommandBus& bus) {
        bus.emplace<OpenCmd>();
        bus.emplace<CloseCmd>();
        bus.emplace<SaveCmd>();
        bus.emplace<SaveAsCmd>();
        bus.emplace<ShowTablesCmd>();
        bus.emplace<DescribeCmd>();
        bus.emplace<PrintCmd>();
        bus.emplace<AddTableCmd>();
        bus.emplace<InsertCmd>();
        bus.emplace<ModifyCmd>();
        bus.emplace<SelectCmd>();
        bus.emplace<UpdateCmd>();
        bus.emplace<DeleteCmd>();
        bus.emplace<AddColumnCmd>();
        bus.emplace<ExportCmd>();
        bus.emplace<LoadCmd>();
        bus.emplace<ExitCmd>();
        bus.emplace<HelpCmd>(); 
    }
};