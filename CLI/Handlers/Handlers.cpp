#include "Handlers.h"
#include "../../Database/Database.h"
#include "../../Table/Table.h"

static bool parseSizeT(const std::string& s, std::size_t& out) 
{
    if (s.empty()) 
        return false;
    std::size_t i = 0, n = s.size();
    std::size_t val = 0;
    for (; i<n; ++i) 
    {
        char c = s[i];
        if (c < '0' || c > '9') 
            return false;
        std::size_t d = std::size_t(c - '0');
        val = val * 10 + d;
    }
    out = val;
    return true;
}

Table* requireTable(Context& ctx, const std::string& name) 
{
    Table* t = ctx.db.getTableByName(name);
    if (!t) 
        std::cout << "Table not found.\n";
    return t;
}

void handleInsert(Context& ctx, const std::vector<std::string>& args) 
{
    if (args.size() < 1) 
    { 
        std::cout << "Usage: insert <table> <values...>\n"; 
        return; 
    }
    const std::string& tableName = args[0];
    Table* table = requireTable(ctx, tableName);
    if (!table) 
        return;

    std::vector<std::string> values;
    values.assign(args.begin()+1, args.end());

    try 
    { 
        table->insertRowFromStrings(values); 
        ctx.out << "Inserted.\n"; 
    }
    catch (const std::exception& e) 
    { 
        std::cout << "Insert failed: " << e.what() << "\n"; 
    }
}

void handleModify(Context& ctx, const std::vector<std::string>& args) 
{
    if (args.size() < 3) 
    { 
        std::cout << "Usage: modify <table> <colIdx> <newType>\n"; 
        return; 
    }
    const std::string& tableName = args[0];
    std::size_t col = 0;
    if (!parseSizeT(args[1], col)) 
    { 
        std::cout << "Invalid column index.\n"; 
        return; 
    }
    const std::string& newType = args[2];

    Table* table = requireTable(ctx, tableName);
    if (!table) 
        return;

    try 
    { 
        table->modifyColumn(col, newType); 
    }
    catch (const std::exception& e) 
    { 
        std::cout << "Modify failed: " << e.what() << "\n"; 
    }
}

void handleSelect(Context& ctx, const std::vector<std::string>& args) 
{
    if (args.size() < 3) 
    { 
        std::cout << "Usage: select <colIdx> <val> <table> [limit] [offset]\n"; 
        return; 
    }
    std::size_t col = 0;

    if (!parseSizeT(args[0], col)) 
    { 
        std::cout << "Invalid column index.\n"; 
        return; 
    }
    const std::string& value = args[1];
    const std::string& tableName = args[2];

    std::size_t limit  = std::size_t(-1); // unlimited
    std::size_t offset = 0;

    if (args.size() >= 4) 
    {
        if (!parseSizeT(args[3], limit)) 
        {
            std::cout << "Invalid limit.\n"; 
            return; 
        }
        if (args.size() >= 5) {
            if (!parseSizeT(args[4], offset)) 
            { 
                std::cout << "Invalid offset.\n"; 
                return; 
            }
        }
    }

    Table* table = requireTable(ctx, tableName);
    if (!table) 
        return;
    try 
    { 
        table->select(col, value, limit, offset); 
    }
    catch (const std::exception& e) 
    { 
        std::cout << "Select failed: " << e.what() << "\n"; 
    }
}

void handleUpdate(Context& ctx, const std::vector<std::string>& args) 
{
    if (args.size() < 5) 
    {
        std::cout << "Usage: update <table> <searchCol> <searchVal> <targetCol> <newVal>\n";
        return;
    }
    const std::string& tableName = args[0];
    std::size_t searchCol = 0, targetCol = 0;

    if (!parseSizeT(args[1], searchCol)) 
    { 
        std::cout << "Invalid searchCol.\n"; 
        return; 
    }
    const std::string& searchVal = args[2];

    if (!parseSizeT(args[3], targetCol)) 
    {
        std::cout << "Invalid targetCol.\n"; 
        return; 
    }
    const std::string& newVal = args[4];

    Table* table = requireTable(ctx, tableName);
    if (!table) 
        return;
    try 
    { 
        table->update(searchCol, searchVal, targetCol, newVal); 
    }
    catch (const std::exception& e) 
    { 
        std::cout << "Update failed: " << e.what() << "\n"; 
    }
}

void handleDelete(Context& ctx, const std::vector<std::string>& args) 
{
    if (args.size() < 3) 
    { 
        std::cout << "Usage: delete <table> <colIdx> <val>\n"; 
        return; 
    }
    const std::string& tableName = args[0];
    std::size_t col = 0;

    if (!parseSizeT(args[1], col)) 
    { 
        std::cout << "Invalid column index.\n"; 
        return; 
    }
    const std::string& value = args[2];

    Table* table = requireTable(ctx, tableName);
    if (!table) 
        return;
    try 
        { 
            table->deleteRows(col, value); 
        }
    catch (const std::exception& e) 
    { 
        std::cout << "Delete failed: " << e.what() << "\n"; 
    }
}

void handleAddColumn(Context& ctx, const std::vector<std::string>& args) 
{
    if (args.size() < 3) 
    { 
        std::cout << "Usage: addcolumn <table> <name> <type>\n"; 
        return; 
    }
    const std::string& tableName = args[0];
    const std::string& colName   = args[1];
    const std::string& colType   = args[2];

    Table* table = requireTable(ctx, tableName);
    if (!table) 
        return;
    try 
    { 
        table->addColumn(colName, colType); 
        ctx.out << "Column '" << colName << "' added.\n"; 
    }
    catch (const std::exception& e) 
    { 
        std::cout << "Add column failed: " << e.what() << "\n"; 
    }
}

void handleExport(Context& ctx, const std::vector<std::string>& args) {
    if (args.size() < 2) 
    { 
        ctx.err << "Usage: export <table> <file>\n"; 
        return; 
    }
    const std::string& tableName = args[0];
    const std::string& filename  = args[1];

    Table* table = requireTable(ctx, tableName);
    if (!table) 
        return;
    try 
    { 
        table->exportToFile(filename); 
    }
    catch (const std::exception& e) 
    { 
        ctx.err << "Export failed: " << e.what() << "\n"; 
    }
}
