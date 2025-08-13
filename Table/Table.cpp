#include <iostream>
#include <fstream>
#include <stdexcept>

#include "Table.h"

Table::Table(const std::string& name) : name(name) {}

void Table::insertRow(const std::vector<Cell*>& data) 
{
    if (data.size() != columns.size()) 
        throw std::invalid_argument("Row size does not match number of columns!");

    rows.emplace_back(data);
}

void Table::insertRowFromStrings(const std::vector<std::string>& values) 
{
    if (values.size() != columns.size())
        throw std::invalid_argument("Row does not match number of columns!");

    std::vector<Cell*> rowData;

    try
    {
        for (size_t i = 0; i < values.size(); ++i) 
        {
            const std::string& colType = columns[i].getType();
            rowData.push_back(createCellFromString(values[i], colType, true));
        }

        rows.emplace_back(rowData);  
    } 
    catch (...) 
    {
        for (size_t i = 0; i < rowData.size(); ++i)
        {
            delete rowData[i];
        }
        throw;  
    }
}

void Table::print() const 
{
    for (size_t i = 0; i < columns.size(); ++i) 
    {
        if (i > 0) std::cout << " | ";
        std::cout << columns[i].getName();
    }
    std::cout << "\n";

    for (size_t i = 0; i < rows.size(); ++i) 
    {
        std::cout << rows[i].toString() << "\n";
    }
}

void Table::describe() const 
{
    for (size_t i = 0; i < columns.size(); ++i) 
    {
        const Column& col = columns[i];
        std::cout << col.getName() << ": " << col.getType() << std::endl;
    }
}

void Table::select(size_t columnIndex, const std::string& value) const 
{
    if (columnIndex >= columns.size())
        throw std::invalid_argument("Invalid column index!");

    const std::string& columnType = columns[columnIndex].getType();
    std::string valToMatch = value;

    if (columnType == "string" && valToMatch.length() >= 2 && valToMatch.front() == '"' && valToMatch.back() == '"') 
        valToMatch = valToMatch.substr(1, valToMatch.length() - 2);

    for (size_t i = 0; i < columns.size(); ++i) 
    {
        if (i > 0) std::cout << " | ";
        std::cout << columns[i].getName();
    }
    std::cout << std::endl;

    for (size_t rowIndex = 0; rowIndex < rows.size(); ++rowIndex) 
    {
        const Row& row = rows[rowIndex];
        const std::vector<Cell*>& cells = row.getCells();

        if (columnIndex >= cells.size() || cells[columnIndex] == nullptr)
            continue;

        std::string cellVal = cells[columnIndex]->toString();

        if (value == "NULL") 
        {
            if (cells[columnIndex]->getType() == CellType::NULLTYPE)
                std::cout << row.toString() << std::endl;
        } else if (columnType == "string") 
        {
            std::string searchLower = toLower(valToMatch);
            std::string cellLower = toLower(cellVal);
            if (cellLower.find(searchLower) != std::string::npos)
                std::cout << row.toString() << std::endl;
        } else 
        {
            if (cellVal == valToMatch)
                std::cout << row.toString() << std::endl;
        }
    }
}

void Table::modifyColumn(size_t columnIndex, const std::string& newType) 
{
    if (columnIndex >= columns.size())
        throw std::invalid_argument("Invalid column index!");

    size_t success = 0;
    size_t failure = 0;

    for (std::size_t i = 0; i < rows.size(); ++i) 
    {
        Row& row = rows[i];
        std::vector<Cell*>& cells = row.getCells();

        Cell* oldCell = cells[columnIndex];
        std::string oldString = oldCell ? oldCell->toString() : "NULL";

        Cell* newCell = nullptr;

        try 
        {
            newCell = createCellFromString(oldString, newType, false); 
            ++success;
        } 
        catch (...) 
        {
            newCell = new NullCell();
            ++failure;
        }

        delete oldCell;
        cells[columnIndex] = newCell;
    }

    columns[columnIndex].setType(newType);
    std::cout << "Modified column to type '" << newType << "'." << std::endl;
    std::cout << "Successfully converted " << success << " cells." << std::endl;
    std::cout << "Failed to convert " << failure << " cells (set to NULL)." << std::endl;
}


void Table::update(size_t searchCol, const std::string& searchVal,
                   size_t targetCol, const std::string& newVal) 
{
    if (searchCol >= columns.size() || targetCol >= columns.size())
        throw std::invalid_argument("Invalid column index!");

    const std::string& targetType = columns[targetCol].getType();
    Cell* prototype = nullptr;

    try 
    {
        prototype = createCellFromString(newVal, targetType);
    } catch (...) 
    {
        throw std::invalid_argument("Invalid new value!");
    }

    size_t updated = 0;

    for (size_t i = 0; i < rows.size(); ++i) 
    {
        Row& row = rows[i];
        std::vector<Cell*>& cells = row.getCells();

        bool match = false;
        if (searchVal == "NULL") 
            match = (cells[searchCol]->getType() == CellType::NULLTYPE);
        else 
            match = (cells[searchCol]->toString() == searchVal);

        if (match) 
        {
            Cell* newCell = nullptr;
            try 
            {
                newCell = prototype->clone(); 
                delete cells[targetCol];      
                cells[targetCol] = newCell;  
                ++updated;
            } catch (...) 
            {
                std::cout << "Memory error while updating row " << i << "." << std::endl;
                continue;
            }
        }
    }
    delete prototype;  
    std::cout << "Updated " << updated << " row(s)." << std::endl;
}

void Table::deleteRows(size_t columnIndex, const std::string& value) 
{
    if (columnIndex >= columns.size()) 
        throw std::invalid_argument("Invalid column index!");

    size_t removed = 0;
    std::vector<Row> newRows;

    for (size_t i = 0; i < rows.size(); ++i) 
    {
        Row& row = rows[i];
        std::vector<Cell*>& cells = row.getCells();

        bool shouldDelete = false;
        if (value == "NULL") 
            shouldDelete = (cells[columnIndex]->getType() == CellType::NULLTYPE);
        else 
            shouldDelete = (cells[columnIndex]->toString() == value);

        if (shouldDelete) 
            ++removed;
        else 
            newRows.push_back(row);
    }
    rows = newRows;
    std::cout << "Deleted " << removed << " row(s)." << std::endl;
}

void Table::addColumn(const std::string& name, const std::string& type) 
{
    for (size_t i = 0; i < columns.size(); ++i) 
    {
        const Column& col = columns[i];
        if (col.getName() == name) 
            throw std::invalid_argument("Column with name '" + name + "' already exists.");
    }
    columns.emplace_back(name, type);

    for (size_t i = 0; i < rows.size(); ++i) 
    {
        Row& row = rows[i];
        row.getCells().push_back(new NullCell());
    }

    std::cout << "Column '" << name << "' added with type '" << type << "'." << std::endl;
}

void Table::exportToFile(const std::string& filename) const 
{
    std::ofstream out(filename);
    if (!out) 
        throw std::runtime_error("Failed to open file!");

    for (std::size_t i = 0; i < columns.size(); ++i) 
    {
        if (i > 0)
            out << ";";  // За да може да се отваря правилно в ексел използвааме ";", вместо ","
    
        out << columns[i].getName();
    }
    out << std::endl;

    for (std::size_t rowIndex = 0; rowIndex < rows.size(); ++rowIndex) 
    {
        const Row& row = rows[rowIndex];
        const std::vector<Cell*>& cells = row.getCells();

        for (std::size_t i = 0; i < cells.size(); ++i) 
        {
            if (i > 0)
                out << ";";  // За да може да се отваря правилно в ексел използвааме ";", вместо ","
            std::string value = cells[i]->toString();
            if (cells[i]->getType() == CellType::STRING) 
                out << "\"" << value << "\"";
            else 
                out << value;        
        }
        out << std::endl;
    }
    out.close();
    std::cout << "Exported table '" << name << "' to file '" << filename << "'." << std::endl;
}

std::string Table::toLower(const std::string& str) 
{
    std::string result;
    result.reserve(str.size());

    for (std::size_t i = 0; i < str.size(); ++i)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
            result += str[i] + ('a' - 'A');
        else
            result += str[i];
    }
    return result;
}