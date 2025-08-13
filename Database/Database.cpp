#include <iostream>
#include <fstream>

#include "Database.h"

void Database::addTable(const Table& table) 
{
    for (size_t i = 0; i < tables.size(); ++i) 
    {
        if (tables[i].getName() == table.getName())
            throw std::invalid_argument("Table with this name already exists.");
    }
    tables.push_back(table);
}

void Database::showTables() const 
{
    if (tables.empty()) 
    {
        std::cout << "No tables available.\n";
        return;
    }

    for (size_t i = 0; i < tables.size(); ++i) 
    {
        const Table& t = tables[i];
        std::cout << t.getName() << "\n";
    }
}

void Database::describe(const std::string& tableName) const 
{
    const Table* table = getTableByName(tableName);
    if (!table) 
    {
        std::cout << "Table '" << tableName << "' not found." << std::endl;
        return;
    }

    table->describe();
}

void Database::clear() 
{
    tables.clear();
}

Table* Database::getTableByName(const std::string& name) 
{
    for (size_t i = 0; i < tables.size(); ++i) 
    {
        if (tables[i].getName() == name)
            return &tables[i];
    }
    return nullptr;
}

const Table* Database::getTableByName(const std::string& name) const 
{
    for (size_t i = 0; i < tables.size(); ++i) 
    {
        if (tables[i].getName() == name)
            return &tables[i];
    }
    return nullptr;
}

void Database::save(const std::string& filename) const 
{
    std::ofstream out(filename);
    if (!out)
        throw std::runtime_error("Failed to open file!");

    for (size_t i = 0; i < tables.size(); ++i) 
    {
        const Table& table = tables[i];
        writeTableHeader(out, table);
        writeTableColumns(out, table);
        writeTableRows(out, table);
        out << "END" << std::endl;
    }

    out.close();
    std::cout << "Database saved to " << filename << std::endl;
}

void Database::writeTableHeader(std::ofstream& out, const Table& table) const
{
    out << "TABLE " << table.getName() << std::endl;
}

void Database::writeTableColumns(std::ofstream& out, const Table& table) const
{
    out << "COLUMNS";
    const std::vector<Column>& columns = table.getColumns();
    for (size_t i = 0; i < columns.size(); ++i) 
    {
        const Column& col = columns[i];
        out << " " << col.getName() << ":" << col.getType();
    }
    out << std::endl;
}

void Database::writeTableRows(std::ofstream& out, const Table& table) const
{
    const std::vector<Row>& rows = table.getRows();
    for (size_t i = 0; i < rows.size(); ++i) 
    {
        const Row& row = rows[i];
        out << "ROW";
        writeRowCells(out, row);
        out << std::endl;
    }
}

void Database::writeRowCells(std::ofstream& out, const Row& row) const
{
    const std::vector<Cell*>& cells = row.getCells();
    for (size_t i = 0; i < cells.size(); ++i) 
    {
        const Cell* c = cells[i];
        out << " ";
        if (c->getType() == CellType::STRING) 
            out << "\"" << c->toString() << "\"";
        else
            out << c->toString();
    }
}

void Database::load(const std::string& filename) 
{
    std::ifstream file(filename);
    if (!file)
        throw std::runtime_error("Failed to open file!");

    tables.clear();
    Table* currentTable = nullptr;

    char buff[1024];

    while (file.getline(buff, sizeof(buff))) 
    {
        std::string line(buff);
        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;

        if (keyword == "TABLE") 
        {
            std::string tableName;
            iss >> tableName;
            currentTable = new Table(tableName);
        }
        else if (keyword == "COLUMNS" && currentTable) 
        {
            std::string nameAndType;
            while (iss >> nameAndType) 
            {
                size_t sep = nameAndType.find(':');
                std::string colName = nameAndType.substr(0, sep);
                std::string colType = nameAndType.substr(sep + 1);
                currentTable->addColumn(colName, colType);
            }
        }
        else if (keyword == "ROW" && currentTable) 
        {
            std::vector<std::string> rawValues;
            std::string value;
            while (iss >> value) 
            {
                rawValues.push_back(value);
            }
            currentTable->insertRowFromStrings(rawValues);
        }
        else if (keyword == "END" && currentTable) 
        {
            tables.push_back(*currentTable);
            delete currentTable;
            currentTable = nullptr;
        }
    }

    std::cout << "Database loaded from " << filename << std::endl;
}