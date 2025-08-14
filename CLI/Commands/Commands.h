#pragma once
#include "../ICommand.h"
#include "../Handlers/Handlers.h"  
#include <string>
#include <vector>

// --- DB meta commands ---
struct ShowTablesCmd : ICommand {
    const char* name() const override { return "showtables"; }
    const char* help() const override { return "showtables - List all tables"; }
    void execute(Context& ctx, const std::vector<std::string>&) override;
};

struct DescribeCmd : ICommand {
    const char* name() const override { return "describe"; }
    const char* help() const override { return "describe <table> - Describe table schema"; }
    void execute(Context& ctx, const std::vector<std::string>& args) override;
};

struct PrintCmd : ICommand {
    const char* name() const override { return "print"; }
    const char* help() const override { return "print <table> - Print all rows of a table"; }
    void execute(Context& ctx, const std::vector<std::string>& args) override;
};

struct OpenCmd : ICommand {
    const char* name() const override { return "open"; }
    const char* help() const override { return "open <file> - Open a database file"; }
    void execute(Context& ctx, const std::vector<std::string>& args) override;
};

struct CloseCmd : ICommand {
    const char* name() const override { return "close"; }
    const char* help() const override { return "close - Close the currently opened database"; }
    void execute(Context& ctx, const std::vector<std::string>&) override;
};

struct SaveCmd : ICommand {
    const char* name() const override { return "save"; }
    const char* help() const override { return "save - Save the database to the current file"; }
    void execute(Context& ctx, const std::vector<std::string>&) override;
};

struct SaveAsCmd : ICommand {
    const char* name() const override { return "saveas"; }
    const char* help() const override { return "saveas <file> - Save the database to a new file"; }
    void execute(Context& ctx, const std::vector<std::string>& args) override;
};

struct LoadCmd : ICommand {
    const char* name() const override { return "load"; }
    const char* help() const override { return "load <file> - Load database (does not set current file)"; }
    void execute(Context& ctx, const std::vector<std::string>& args) override;
};

// --- table & row commands (delegate to Handlers) ---
struct AddTableCmd : ICommand {
    const char* name() const override { return "addtable"; }
    const char* help() const override { return "addtable <table> - Add new table"; }
    void execute(Context& ctx, const std::vector<std::string>& args) override;
};

struct InsertCmd : ICommand {
    const char* name() const override { return "insert"; }
    const char* help() const override { return "insert <table> <values...>"; }
    void execute(Context& ctx, const std::vector<std::string>& args) override;
};

struct ModifyCmd : ICommand {
    const char* name() const override { return "modify"; }
    const char* help() const override { return "modify <table> <colIdx> <newType>"; }
    void execute(Context& ctx, const std::vector<std::string>& args) override;
};

struct SelectCmd : ICommand {
    const char* name() const override { return "select"; }
    const char* help() const override { return "select <colIdx> <val> <table> [limit] [offset]"; }
    void execute(Context& ctx, const std::vector<std::string>& args) override;
};

struct UpdateCmd : ICommand {
    const char* name() const override { return "update"; }
    const char* help() const override { return "update <table> <searchCol> <searchVal> <targetCol> <newVal>"; }
    void execute(Context& ctx, const std::vector<std::string>& args) override;
};

struct DeleteCmd : ICommand {
    const char* name() const override { return "delete"; }
    const char* help() const override { return "delete <table> <colIdx> <val>"; }
    void execute(Context& ctx, const std::vector<std::string>& args) override;
};

struct AddColumnCmd : ICommand {
    const char* name() const override { return "addcolumn"; }
    const char* help() const override { return "addcolumn <table> <name> <type>"; }
    void execute(Context& ctx, const std::vector<std::string>& args) override;
};

struct ExportCmd : ICommand {
    const char* name() const override { return "export"; }
    const char* help() const override { return "export <table> <file>"; }
    void execute(Context& ctx, const std::vector<std::string>& args) override;
};

// --- misc ---
struct ExitCmd : ICommand {
    const char* name() const override { return "exit"; }
    const char* help() const override { return "exit - Exit the program"; }
    void execute(Context& , const std::vector<std::string>&) override { std::exit(0); }
};

struct HelpCmd : ICommand {
    const char* name() const override { return "help"; }
    const char* help() const override { return "help - Show available commands"; }
    void execute(Context& ctx, const std::vector<std::string>&) override;
};
