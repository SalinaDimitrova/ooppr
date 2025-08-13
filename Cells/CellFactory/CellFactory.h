#pragma once

#include <sstream>
#include <stdexcept>
#include <cctype>

#include "../Cell.h"
#include "../IntCell/IntCell.h"
#include "../DoubleCell/DoubleCell.h"
#include "../StringCell/StringCell.h"
#include "../DateCell/DateCell.h"
#include "../NullCell/NullCell.h"

std::string parseEscapedString(const std::string& quoted);
bool parseDateString(const std::string& value, int& y, int& m, int& d);
Cell* createCellFromString(const std::string& value, const std::string& type, bool strictStringParsing = true);

