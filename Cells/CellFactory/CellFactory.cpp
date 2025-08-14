#include "CellFactory.h"
#include <stdexcept>
#include <string>

static std::string toLowerCopy(const std::string& s) 
{
    std::string r;
    r.reserve(s.size());
    for (char ch : s) 
    {
        unsigned char uc = static_cast<unsigned char>(ch);
        if (uc >= 'A' && uc <= 'Z') 
        r.push_back(static_cast<char>(uc + ('a' - 'A')));
        else                        
            r.push_back(ch);
    }
    return r;
}

std::string parseEscapedString(const std::string& quoted) 
{
    std::string result;
    for (size_t i = 1; i + 1 < quoted.length(); ++i) 
    {
        if (quoted[i] == '\\' && i + 1 < quoted.length() - 1) 
        {
            if (quoted[i + 1] == '"' || quoted[i + 1] == '\\') 
            {
                result += quoted[i + 1];
                ++i;
            } else 
                result += quoted[i];
        } else 
            result += quoted[i];
    }
    return result;
}

bool parseDateString(const std::string& value, int& y, int& m, int& d) 
{
    if (value.size() != 10 || value[4] != '-' || value[7] != '-') 
        return false;

    try 
    {
        y = std::stoi(value.substr(0, 4));
        m = std::stoi(value.substr(5, 2));
        d = std::stoi(value.substr(8, 2));

        if (m < 1 || m > 12 || d < 1 || d > 31) 
            return false;

        return true;
    } catch (...) 
    { 
        return false; 
    }
}

Cell* createCellFromString(const std::string& value, CellType type, bool strictStringParsing) 
{
    if (value == "NULL")
         return new NullCell();

    try 
    {
        switch (type) 
        {
            case CellType::INT: 
            {
                int v = std::stoi(value);
                return new IntCell(v);
            }
            case CellType::DOUBLE: 
            {
                double v = std::stod(value);
                return new DoubleCell(v);
            }
            case CellType::STRING: 
            {
                if (!strictStringParsing) 
                    return new StringCell(value);
                if (value.length() >= 2 && value.front() == '"' && value.back() == '"')
                    return new StringCell(parseEscapedString(value));
                throw std::invalid_argument("String must be quoted.");
            }
            case CellType::DATE: 
            {
                int y, m, d;
                if (parseDateString(value, y, m, d)) 
                    return new DateCell(d, m, y);
                throw std::invalid_argument("Invalid date format.");
            }
            case CellType::NULLTYPE:
                return new NullCell();
        }
    } catch (...) 
    {
        throw std::invalid_argument("Failed to parse value '" + value + "'");
    }
    throw std::invalid_argument("Unsupported type.");
}

Cell* createCellFromString(const std::string& value, const std::string& typeString, bool strictStringParsing) {
    std::string t = toLowerCopy(typeString);
    CellType ct;
    if      (t == "int")    
        ct = CellType::INT;
    else if (t == "double") 
        ct = CellType::DOUBLE;
    else if (t == "string") 
        ct = CellType::STRING;
    else if (t == "date")   
        ct = CellType::DATE;
    else if (t == "null")   
        ct = CellType::NULLTYPE;
    else 
        throw std::invalid_argument("Unknown type: " + typeString);

    return createCellFromString(value, ct, strictStringParsing);
}
