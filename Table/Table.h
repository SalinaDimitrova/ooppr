#pragma once

#include <string>
#include <vector>

#include "../Column/Column.h"
#include "../Row/Row.h"
#include "../Cells/CellFactory/CellFactory.h"

class Table 
{
public:
    Table(const std::string& name);

    void describe() const;
    void print() const;
    void exportToFile(const std::string& filename) const;


    const std::string& getName() const { return name; }
    const std::vector<Column>& getColumns() const { return columns; }
    const std::vector<Row>& getRows() const { return rows; }
    std::vector<Row>& getRows() { return rows; }

    void insertRow(const std::vector<Cell*>& data);
    void insertRowFromStrings(const std::vector<std::string>& values);
    void select(size_t columnIndex, const std::string& value) const;
    void modifyColumn(size_t columnIndex, const std::string& newType);
    void addColumn(const std::string& name, const std::string& type);
    void update(size_t searchCol, const std::string& searchVal,
                size_t targetCol, const std::string& newVal);
    void deleteRows(size_t columnIndex, const std::string& value);

private:
    std::string name;
    std::vector<Column> columns;
    std::vector<Row> rows;

    static std::string toLower(const std::string& str);
};

