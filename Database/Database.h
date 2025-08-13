#pragma once

#include <vector>
#include <string>

#include "../Table/Table.h"

class Database 
{
public:
    void addTable(const Table& table);
    void showTables() const;
    void describe(const std::string& tableName) const;
    void clear();

    Table* getTableByName(const std::string& name);
    const Table* getTableByName(const std::string& name) const;

    void load(const std::string& filename); 
    void save(const std::string& filename) const;


private:
    std::vector<Table> tables;

    void writeTableHeader(std::ofstream& out, const Table& table) const;
    void writeTableColumns(std::ofstream& out, const Table& table) const;
    void writeTableRows(std::ofstream& out, const Table& table) const;
    void writeRowCells(std::ofstream& out, const Row& row) const;
};


