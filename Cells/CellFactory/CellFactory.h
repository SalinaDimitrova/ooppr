#pragma once
#include <string>
#include "Cells/Cell.h"
#include "Cells/IntCell/IntCell.h"
#include "Cells/DoubleCell/DoubleCell.h"
#include "Cells/StringCell/StringCell.h"
#include "Cells/DateCell/DateCell.h"
#include "Cells/NullCell/NullCell.h"


std::string parseEscapedString(const std::string& quoted);
bool parseDateString(const std::string& value, int& y, int& m, int& d);

// enum-based
Cell* createCellFromString(const std::string& value, CellType type, bool strictStringParsing = true);

// string-based – delegates to enum version
Cell* createCellFromString(const std::string& value, const std::string& typeString, bool strictStringParsing = true);
