#ifndef TOOLS__HPP
#define TOOLS__HPP

#include "externs/tokenizer/tokenizer.hpp"

#include <cstdio>
#include <iostream>
#include <vector>
#include <string>

char* file_read_all_text(const char* _filePath)
{
    FILE* file = fopen(_filePath, "r");

    if (file != nullptr)
    {
        fseek(file, 0, SEEK_END);

        size_t size = ftell(file);
        char* content = new char[size];

        rewind(file);
        fread(content, sizeof(char), size, file);

        return content;
    }

    return nullptr;
}

void skip_until(tokenizer& _tokenizer, const std::string& _str)
{
    while (!_tokenizer.finished() && _tokenizer.next() != _str) { }
}

size_t find_token_pos(
    const std::vector<std::string>& _tokens,
    const std::string& _token, size_t _start = 0)
{
    for (size_t i = _start; i < _tokens.size(); i++)
    {
        if (_tokens[i] == _token)
        {
            return _start;
        }
    }
    
    return std::string::npos;
}

#endif // !TOOLS__HPP