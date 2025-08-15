#include <iostream>
#include <fstream>

#include "Database.h"

static inline bool is_space(unsigned char c) 
{
    return c==' ' || c=='\t' || c=='\r' || c=='\n' || c=='\f' || c=='\v';
}

static std::string trim_copy(const std::string& s) 
{
    std::size_t a = 0;
    std::size_t b = s.size();
    while (a < b && is_space((unsigned char)s[a])) 
        ++a;
    while (b > a && is_space((unsigned char)s[b - 1])) 
        --b;

    return s.substr(a, b - a);
}

static std::vector<std::string> split_ws_copy(const std::string& s) 
{
    std::vector<std::string> out;
    std::size_t i = 0;
    const std::size_t n = s.size();
    while (i < n) 
    {
        while (i < n && is_space((unsigned char)s[i])) 
            ++i;
        if (i >= n) 
            break;
        std::size_t j = i;
        while (j < n && !is_space((unsigned char)s[j])) 
            ++j;
        out.push_back(s.substr(i, j - i));
        i = j;
    }
    return out;
}

static std::vector<std::string> parse_values_line(const std::string& line) 
{
    std::vector<std::string> vals;
    size_t i = 0, n = line.size();
    while (i < n) 
    {
        while (i < n && is_space((unsigned char)line[i])) 
            ++i;
            
        if (i >= n) 
            break;

        if (line[i] != '"') 
        {
            size_t j = i;
            while (j < n && !is_space((unsigned char)line[j])) 
                ++j;
            vals.emplace_back(line.substr(i, j - i));
            i = j;
        } else 
        {
            ++i;
            std::string out;
            while (i < n) {
                char c = line[i++];
                if (c == '\\') 
                {
                    if (i < n) 
                    {
                        char d = line[i++];
                        if (d == '"' || d == '\\') 
                            out.push_back(d);
                        else 
                        { 
                            out.push_back('\\'); 
                            out.push_back(d); 
                        }
                    } else 
                        out.push_back('\\');
                } else if (c == '"') 
                    break; 
                else
                    out.push_back(c);
            }
            vals.emplace_back(out);
        }
    }
    return vals;
}

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
        out << " " << col.getName() << ":" << Column::typeToString(col.getType());
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
    std::string (*escape)(const std::string&) =
        [](const std::string& s) -> std::string 
        {
            std::string r;
            r.reserve(s.size());
            for (char ch : s) 
            {
                if (ch == '"' || ch == '\\') r.push_back('\\');
                r.push_back(ch);
            }
            return r;
        };

    for (std::size_t i = 0; i < row.size(); ++i) 
    {
        const Cell* c = row.cellAt(i);
        out << " ";

        if (!c) 
        {
            out << "NULL";
            continue;
        }

        if (c->getType() == CellType::STRING)
            out << '"' << escape(c->toString()) << '"';
        else
            out << c->toString();
    }
}

void Database::load(const std::string& filename) {
    std::ifstream in(filename.c_str());
    if (!in) throw std::runtime_error("Cannot open file for reading.");

    clear();
    std::string line;

    while (true) {
        std::string firstLine;
        while (std::getline(in, line)) {
            firstLine = trim_copy(line);
            if (!firstLine.empty()) break;
        }
        if (!in) break; 

        std::vector<std::string> first = split_ws_copy(firstLine);
        if (first.empty()) continue;

        std::string tableName;
        if (first.size() >= 2 && first[0] == "TABLE")
            tableName = first[1];
        else if (first.size() == 1)
            tableName = first[0];
        else
            throw std::runtime_error("Invalid table header: " + firstLine);

        if (!std::getline(in, line))
            throw std::runtime_error("Invalid file: missing schema line for table '" + tableName + "'.");

        std::string schemaLine = trim_copy(line);
        if (schemaLine.empty())
            throw std::runtime_error("Invalid file: empty schema line for table '" + tableName + "'.");

        std::vector<std::string> schemaTokens = split_ws_copy(schemaLine);
        std::size_t startIndex = 0;
        if (!schemaTokens.empty() && schemaTokens[0] == "COLUMNS") 
            startIndex = 1;

        Table t(tableName);
        for (std::size_t k = startIndex; k < schemaTokens.size(); ++k) 
        {
            const std::string& tok = schemaTokens[k];
            std::size_t p = tok.find(':');
            if (p == std::string::npos)
                throw std::runtime_error("Invalid schema token: " + tok);
            t.addColumn(tok.substr(0, p), tok.substr(p + 1));
        }

        while (std::getline(in, line)) 
        {
            std::string trimmed = trim_copy(line);
            if (trimmed.empty() || trimmed == "ROWS") 
                continue;
            if (trimmed == "END") 
                break; 

            std::string rowText = trimmed;
            if (trimmed.rfind("ROW", 0) == 0) 
            {
                rowText = trim_copy(trimmed.substr(3));
                if (rowText.empty()) 
                    continue; 
            }

            std::vector<std::string> values = parse_values_line(rowText);
            if (values.size() != t.getColumnCount())
                throw std::runtime_error("Row has wrong number of fields in table '" + tableName + "'.");
            t.insertRowFromStrings(values);
        }

        addTable(t);

        if (!in) 
            break;
    }
}
