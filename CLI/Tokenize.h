#pragma once
#include <string>
#include <vector>

inline bool is_space_char(unsigned char c) 
{
    return c==' ' || c=='\t' || c=='\n' || c=='\r' || c=='\f' || c=='\v';
}

inline std::vector<std::string> tokenize(const std::string& line) 
{
    std::vector<std::string> out;
    std::size_t i = 0;
    const std::size_t n = line.size();

    while (i < n) {
        while (i < n && is_space_char(static_cast<unsigned char>(line[i]))) 
            ++i;
        if (i >= n) 
            break;

        if (line[i] != '"') 
        {
            std::size_t j = i;
            while (j < n && !is_space_char(static_cast<unsigned char>(line[j]))) 
                ++j;
            out.push_back(line.substr(i, j - i));
            i = j;
        } else 
        {
            ++i; // skip opening quote
            std::string t;
            while (i < n) 
            {
                char c = line[i++];
                if (c == '\\') 
                {
                    if (i < n) 
                    {
                        char d = line[i++];
                        if (d == '"' || d == '\\') 
                            t.push_back(d);
                        else 
                        { 
                            t.push_back('\\'); 
                            t.push_back(d); 
                        }
                    } else 
                        t.push_back('\\');
                } else if (c == '"') 
                    break;
                else 
                    t.push_back(c);
            }
            out.push_back(t);
        }
    }
    return out;
}
