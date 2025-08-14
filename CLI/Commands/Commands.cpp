#include "Commands.h"
#include "../Handlers/Handlers.h"
#include "../../Database/Database.h"
#include "../../Table/Table.h"

// --- DB meta commands ---

void ShowTablesCmd::execute(Context& ctx, const std::vector<std::string>&) 
{
    ctx.db.showTables();
}

void DescribeCmd::execute(Context& ctx, const std::vector<std::string>& args) 
{
    if (args.size() < 1) 
    { 
        std::cout << "Usage: describe <table>\n"; 
        return; 
    }
    ctx.db.describe(args[0]);
}

void PrintCmd::execute(Context& ctx, const std::vector<std::string>& args) 
{
    if (args.size() < 1) 
    { 
        std::cout << "Usage: print <table>\n"; 
        return; 
    }
    Table* t = requireTable(ctx, args[0]);
    if (!t) 
        return;
    t->print();
}

// --- file commands ---

void OpenCmd::execute(Context& ctx, const std::vector<std::string>& args) 
{
    if (args.size() < 1) 
    { 
        std::cout << "Usage: open <file>\n"; 
        return; 
    }
    const std::string& filename = args[0];
    ctx.db.load(filename);
    ctx.currentFile = filename;
}

void CloseCmd::execute(Context& ctx, const std::vector<std::string>&) 
{
    ctx.db.clear();
    ctx.currentFile.clear();
    ctx.out << "Database closed.\n";
}

void SaveCmd::execute(Context& ctx, const std::vector<std::string>&) 
{
    if (ctx.currentFile.empty()) {
        std::cout << "No file loaded. Use 'saveas' instead.\n";
        return;
    }
    ctx.db.save(ctx.currentFile);
}

void SaveAsCmd::execute(Context& ctx, const std::vector<std::string>& args) 
{
    if (args.size() < 1) 
    { 
        std::cout << "Usage: saveas <file>\n"; 
        return; 
    }
    const std::string& filename = args[0];
    ctx.db.save(filename);
    ctx.currentFile = filename;
}

void LoadCmd::execute(Context& ctx, const std::vector<std::string>& args) 
{
    if (args.size() < 1) 
    { 
        std::cout << "Usage: load <file>\n"; 
        return; 
    }
    const std::string& filename = args[0];
    ctx.db.load(filename); // does not set currentFile by design
}

// --- table & row commands (delegate to Handlers) ---

void AddTableCmd::execute(Context& ctx, const std::vector<std::string>& args) 
{

    if (args.size() < 1) 
    { 
        std::cout << "Usage: addtable <table>\n"; 
        return; 
    }

    try 
    {
        Table newTable(args[0]);
        ctx.db.addTable(newTable);
        ctx.out << "Table '" << args[0] << "' created.\n";
    } catch (const std::exception& e) 
    {
        std::cout << "Add table failed: " << e.what() << "\n";
    }
}

void InsertCmd::execute(Context& ctx, const std::vector<std::string>& args) 
{
    handleInsert(ctx, args);
}

void ModifyCmd::execute(Context& ctx, const std::vector<std::string>& args) 
{
    handleModify(ctx, args);
}

void SelectCmd::execute(Context& ctx, const std::vector<std::string>& args) 
{
    handleSelect(ctx, args);
}

void UpdateCmd::execute(Context& ctx, const std::vector<std::string>& args) 
{
    handleUpdate(ctx, args);
}

void DeleteCmd::execute(Context& ctx, const std::vector<std::string>& args) 
{
    handleDelete(ctx, args);
}

void AddColumnCmd::execute(Context& ctx, const std::vector<std::string>& args) 
{
    handleAddColumn(ctx, args);
}

void ExportCmd::execute(Context& ctx, const std::vector<std::string>& args) 
{
    handleExport(ctx, args);
}

void HelpCmd::execute(Context& ctx, const std::vector<std::string>&)
{
    ctx.out
        << "Available commands:\n"
        << "open <file>       - Open a database file\n"
        << "close             - Close the currently opened database\n"
        << "save              - Save the database to the current file\n"
        << "saveas <file>     - Save the database to a new file\n"
        << "showtables        - List all tables\n"
        << "describe <table>  - Describe table schema\n"
        << "print <table>     - Print all rows of a table\n"
        << "addtable <table>  - Add new table\n"
        << "insert <table> <values...> - Insert row into table\n"
        << "select <colIdx> <val> <table> [limit] [offset] - Select rows\n"
        << "update <table> <searchCol> <searchVal> <targetCol> <newVal>\n"
        << "delete <table> <colIdx> <val> - Delete rows\n"
        << "addcolumn <table> <name> <type> - Add a column\n"
        << "modify <table> <colIdx> <newType> - Change column type\n"
        << "export <table> <file> - Export table to CSV\n"
        << "exit              - Exit the program\n";
}
