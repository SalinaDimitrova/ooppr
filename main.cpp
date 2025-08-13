#include <iostream>
#include <vector>
#include <string>

#include "Database/Database.h"
#include "Cells/Cell.h"
#include "Cells/IntCell/IntCell.h"
#include "Cells//DoubleCell/DoubleCell.h"
#include "Cells/StringCell/StringCell.h"
#include "Cells/DateCell/DateCell.h"
#include "Cells/NullCell/NullCell.h"
#include "Cells/CellFactory/CellFactory.h"

std::string currentFile;

Database db;

void showHelp()
{
    std::cout << "Available commands:\n";
    std::cout << "open <file>       - Open a database file\n";
    std::cout << "close             - Close the currently opened database\n";
    std::cout << "save              - Save the database to the current file\n";
    std::cout << "saveas <file>     - Save the database to a new file\n";
    std::cout << "showtables        - List all tables\n";
    std::cout << "describe <table>  - Describe table schema\n";
    std::cout << "print <table>     - Print all rows of a table\n";
    std::cout << "addtable <table> - Add new table\n";
    std::cout << "insert <table> <values...> - Insert row into table\n";
    std::cout << "select <colIdx> <val> <table> - Select rows\n";
    std::cout << "update <table> <searchCol> <searchVal> <targetCol> <newVal>\n";
    std::cout << "delete <table> <colIdx> <val> - Delete rows\n";
    std::cout << "addcolumn <table> <name> <type> - Add a column\n";
    std::cout << "modify <table> <colIdx> <newType> - Change column type\n";
    std::cout << "export <table> <file> - Export table to CSV\n";
    std::cout << "exit              - Exit the program\n";

}
Table* requireTable(const std::string& name)
{
     Table* table = db.getTableByName(name);
    if (!table) 
        std::cout << "Table not found.\n";
    return table;
}
void handleInsert(std::istringstream& iss)
{
    std::string tableName;
    iss >> tableName;

    Table* table = requireTable(tableName);
    if (!table) 
        return;

    std::vector<std::string> values;
    std::string val;
    while (iss >> std::ws && std::getline(iss, val, ' ')) 
    {
        if (!val.empty())
            values.push_back(val);
    }

    try 
    {
        table->insertRowFromStrings(values);
        std::cout << "Inserted.\n";
    } catch (const std::exception& e) {
        std::cout << "Insert failed: " << e.what() << "\n";
    }
}
void handleModify(std::istringstream& iss)
{
    std::string tableName;
    std::size_t columnIndex;
    std::string newType;

    iss >> tableName >> columnIndex >> newType;

    Table* table = requireTable(tableName);
    if (!table) 
        return;
    else
        table->modifyColumn(columnIndex, newType);
}
void handleSelect(std::istringstream& iss)
{
    size_t columnIndex;
    std::string value, tableName;
    iss >> columnIndex >> value >> tableName;

    Table* table = requireTable(tableName);
    if (!table) 
        return;
    else
        table->select(columnIndex, value);
}
void handleUpdate(std::istringstream& iss)
{
    std::string tableName;
    size_t searchCol, targetCol;
    std::string searchVal, newVal;

    iss >> tableName >> searchCol >> searchVal >> targetCol >> newVal;

     Table* table = requireTable(tableName);
    if (!table) 
        return;
    else 
        table->update(searchCol, searchVal, targetCol, newVal);
}
void handleDelete(std::istringstream& iss)
{
    std::string tableName;
    size_t colIndex;
    std::string value;

    iss >> tableName >> colIndex >> value;

    Table* table = requireTable(tableName);
    if (!table) 
        return;
    else
        table->deleteRows(colIndex, value);
}
void handleAddColumn(std::istringstream& iss)
{
    std::string tableName, colName, colType;
    iss >> tableName >> colName >> colType;

    Table* table = db.getTableByName(tableName);
    if (!table) {
        std::cout << "Table not found.\n";
    } else {
        table->addColumn(colName, colType);
    }
}
void handleExport(std::istringstream& iss)
{
    std::string tableName, filename;
    iss >> tableName >> filename;

    Table* table = requireTable(tableName);
    if (!table) 
        return;
    else 
        table->exportToFile(filename);
}

void executeCommand(const std::string& line) 
{
    std::istringstream iss(line);
    std::string command;
    iss >> command;

    if (command == "showtables") 
    {
        db.showTables();
    } 
    else if (command == "describe") 
    {
        std::string tableName;
        iss >> tableName;
        db.describe(tableName);
    }
    else if (command == "print") 
    {
        std::string tableName;
        iss >> tableName;
        
        Table* table = requireTable(tableName);
        if (!table) 
            return;
        else 
            table->print();
    }
    else if (command == "open") 
    {
        std::string filename;
        iss >> filename;

        if (filename.empty()) 
        {
            std::cout << "Missing filename.\n";
            return;
        }

        db.load(filename);
        currentFile = filename;
    }

    else if (command == "close") 
    {
        db.clear();
        currentFile.clear();
        std::cout << "Database closed.\n";
    }
    else if (command == "save") 
    {
        if (currentFile.empty()) 
        {
            std::cout << "No file loaded. Use 'saveas' instead.\n";
            return;
        }
        db.save(currentFile);
    }
    else if (command == "saveas") 
    {
        std::string filename;
        iss >> filename;

        if (filename.empty()) 
        {
            std::cout << "Missing filename.\n";
            return;
        }

        db.save(filename);
        currentFile = filename;
    }
    else if (command == "help") 
    {
        showHelp();
    }
    else if (command == "addtable") 
    {
        std::string tableName;
        iss >> tableName;

        if (tableName.empty()) 
        {
            std::cout << "Missing table name.\n";
        } else 
        {
            Table newTable(tableName);
            db.addTable(newTable);
            std::cout << "Table '" << tableName << "' created.\n";
        }
    }
    else if (command == "insert") handleInsert(iss);
    else if (command == "modify") handleModify(iss);
    else if (command == "select") handleSelect(iss);
    else if (command == "update") handleUpdate(iss);
    else if (command == "delete") handleDelete(iss);
    else if (command == "addcolumn") handleAddColumn(iss);
    else if (command == "export") handleExport(iss);
    else if (command == "load") 
    {
        std::string filename;
        iss >> filename;
        db.load(filename);
    }
    else if (command == "exit") 
        std::exit(0);
    else 
        std::cout << "Unknown command.\n";
}

int main() 
{
    std::cout << "MiniDB REPL started. Type 'exit' to quit.\n";

    Table users("users");
    users.addColumn("id", "int");
    users.addColumn("name", "string");
    users.addColumn("dob", "date");
    db.addTable(users);

    std::string line;
    while (true) 
    {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (!line.empty()) 
        {
            try 
            {
                executeCommand(line);
            } catch (const std::exception& e) 
            {
                std::cout << "Error: " << e.what() << "\n";
            }
        }
    }

    return 0;
}
