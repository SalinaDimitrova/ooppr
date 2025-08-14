#include <iostream>
#include <fstream>
#include <stdexcept>

#include "Table.h"

static std::string lowerCopy(const std::string& s) 
{
    std::string r;
    r.reserve(s.size());
    for (char ch : s) 
    {
        if (ch >= 'A' && ch <= 'Z') 
            r.push_back(char(ch + ('a' - 'A')));
        else                        
            r.push_back(ch);
    }
    return r;
}

Table::Table(const std::string& name) : name(name) {}

void Table::insertRow(const std::vector<Cell*>& data) 
{
    if (data.size() != columns.size()) 
        throw std::invalid_argument("Row size does not match number of columns!");

    rows.emplace_back(data);
}

void Table::insertRowFromStrings(const std::vector<std::string>& values) {
    if (values.size() != columns.size())
        throw std::invalid_argument("Row does not match number of columns!");

    std::vector<Cell*> rowData;
    try 
    {
        for(size_t i = 0; i < values.size(); ++i) 
        {
            CellType ct = columns[i].getType();
            rowData.push_back(createCellFromString(values[i], ct, /*strict=*/false));
        }
        rows.emplace_back(rowData);
    } catch (...) 
    {
        for (Cell* c : rowData) 
            delete c;
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
    if (columns.empty()) 
    {
        std::cout << "(no columns)\n";
        return;
    }

    for (const Column& col : columns) 
    {
        std::cout << col.getName() << ": "
                  << Column::typeToString(col.getType()) << '\n';
    }
}

void Table::select(size_t columnIndex, const std::string& value,
                   size_t limit, size_t offset) const
{
    if (columnIndex >= columns.size())
        throw std::invalid_argument("Invalid column index!");

    CellType colType = columns[columnIndex].getType();
    std::string valToMatch = value;

    if (colType == CellType::STRING &&
        valToMatch.size() >= 2 && valToMatch.front() == '"' && valToMatch.back() == '"') 
    {
        valToMatch = valToMatch.substr(1, valToMatch.size() - 2);
    }

    for (size_t i = 0; i < columns.size(); ++i) 
    {
        if (i > 0) std::cout << " | ";
        std::cout << columns[i].getName();
    }
    std::cout << '\n';

    size_t seen = 0, printed = 0;

    for (const Row& row : rows) {
        if (columnIndex >= row.size()) 
            continue;
        const Cell* c = row.cellAt(columnIndex);
        if (!c) 
            continue;

        bool match = false;
        if (value == "NULL") 
        {
            match = (c->getType() == CellType::NULLTYPE);
        } else if (colType == CellType::STRING) 
        {
                std::string hay = lowerCopy(c->toString());
                std::string needle = lowerCopy(valToMatch);
                match = (hay.find(needle) != std::string::npos);
        }
        else 
            match = (c->toString() == valToMatch);

        if (match) 
        {
            if (seen++ < offset) continue;
            if (printed >= limit) break;
            std::cout << row.toString() << '\n';
            ++printed;
        }
    }
}

void Table::modifyColumn(std::size_t columnIndex, const std::string& newTypeStr)
{
    modifyColumn(columnIndex, Column::typeFromString(newTypeStr));
}

void Table::modifyColumn(std::size_t columnIndex, CellType newType)
{
    if (columnIndex >= columns.size())
        throw std::invalid_argument("Invalid column index!");

    std::size_t success = 0, failure = 0;

    for (Row& row : rows)
    {
        const Cell* oldCell = row.cellAt(columnIndex);
        const std::string oldString = oldCell ? oldCell->toString() : "NULL";

        Cell* newCell = nullptr;
        try 
        {
            newCell = createCellFromString(oldString, newType, /*strictStringParsing=*/false);
            ++success;
        } catch (...) {
            newCell = new NullCell();
            ++failure;
        }
        row.replaceCell(columnIndex, newCell);
    }

    columns[columnIndex].setType(newType);

    std::cout << "Modified column to type '"
              << Column::typeToString(newType) << "'.\n"
              << "Successfully converted " << success << " cells.\n"
              << "Failed to convert " << failure << " cells (set to NULL).\n";
}

void Table::update(size_t searchCol, const std::string& searchVal,
                   size_t targetCol, const std::string& newVal) 
{
    if (searchCol >= columns.size() || targetCol >= columns.size())
        throw std::invalid_argument("Invalid column index!");

    CellType targetType = columns[targetCol].getType();

    Cell* prototype = nullptr;
    try 
    {
        prototype = createCellFromString(newVal, targetType, false);
    } catch (...) 
    {
        throw std::invalid_argument("Invalid new value!");
    }

    size_t updated = 0;

    for (size_t i = 0; i < rows.size(); ++i) 
    {
        Row& row = rows[i];
        if (searchCol >= row.size() || targetCol >= row.size())
            continue;

        Cell* sc = row.cellAt(searchCol);
        bool match = false;

        if (searchVal == "NULL") 
            match = (sc->getType() == CellType::NULLTYPE);
        else                    
            match = (sc->toString() == searchVal);

        if (match) 
        {
            try 
            {
                Cell* newCell = prototype->clone();
                row.replaceCell(targetCol, newCell);
                ++updated;
            } catch (...) 
            {
                std::cout << "Memory error while updating row " << i << ".\n";
                continue;
            }
        }
    }
    delete prototype;
    std::cout << "Updated " << updated << " row(s).\n";
}

void Table::deleteRows(size_t columnIndex, const std::string& value) 
{
    if (columnIndex >= columns.size())
        throw std::invalid_argument("Invalid column index!");

    size_t removed = 0;
    std::vector<Row> kept;

    kept.reserve(rows.size());

    for (Row& row : rows) {
        if (columnIndex >= row.size()) 
        {
            kept.push_back(row); 
            continue;
        }
            
        Cell* c = row.cellAt(columnIndex);
        bool shouldDelete = false;

        if (value == "NULL") 
            shouldDelete = (c->getType() == CellType::NULLTYPE);
        else                 
            shouldDelete = (c->toString() == value);

        if (shouldDelete) 
            ++removed;
        else              
            kept.push_back(row); 
    }
    rows = std::move(kept);
    std::cout << "Deleted " << removed << " row(s).\n";
}

void Table::addColumn(const std::string& name, const std::string& typeStr) 
{
    for (const Column& col : columns) 
    {
        if (col.getName() == name)
            throw std::invalid_argument("Column with name '" + name + "' already exists.");
    }
    columns.emplace_back(name, Column::typeFromString(typeStr));

    for (Row& row : rows) 
        row.appendNull();

    std::cout << "Column '" << name << "' added with type '"
              << Column::typeToString(Column::typeFromString(typeStr)) << "'.\n";
}

void Table::exportToFile(const std::string& filename) const 
{
    std::ofstream out(filename);
    if (!out) 
        throw std::runtime_error("Failed to open file!");

    for (std::size_t i = 0; i < columns.size(); ++i) 
    {
        if (i > 0) out << ";";
        out << columns[i].getName();
    }
    out << '\n';

    for (const Row& row : rows) 
    {
        for (std::size_t i = 0; i < row.size(); ++i) 
        {
            if (i > 0) 
                out << ";";

            const Cell* c = row.cellAt(i);
            std::string value = c ? c->toString() : "NULL";

            if (c && c->getType() == CellType::STRING)
                out << '"' << value << '"';
            else
                out << value;
        }
        out << '\n';
    }
    out.close();
    std::cout << "Exported table '" << name << "' to file '" << filename << "'.\n";
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