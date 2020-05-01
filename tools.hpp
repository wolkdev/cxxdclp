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

        fclose(file);

        return content;
    }

    return nullptr;
}

void skip_until(tokenizer& _tokenizer, const std::string& _str)
{
    while (!_tokenizer.finished() && _tokenizer.next() != _str) { }
}

char get_special_char(char _char)
{
    switch (_char)
    {
        case 'n': return '\n';
        case 'r': return '\r';
        case 't': return '\t';
        case '0': return '\0';
        default: return _char;
    }
}

std::string get_string(const char* _str)
{
    int i = 1;
    bool special;
    std::string str;

    if (_str[0] != '\"' || _str[0] == '\0')
    {
        return "";
    }

    while (_str[i] != '\0' && (special || _str[i] != '\"'))
    {
        if (!special && _str[i] == '\\')
        {
            special = true;
        }
        else
        {
            if (special)
            {
                str += get_special_char(_str[i]);
            }
            else
            {
                str += _str[i];
            }
        }

        i++;
    }

    return str;
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

bool have_token(
    const std::vector<std::string>& _tokens,
    const std::string& _token)
{
    return find_token_pos(_tokens, _token) != std::string::npos;
}

bool is_digit(const std::string& _str)
{
    for (size_t i = 0; i < _str.size(); i++)
    {
        if (_str[i] < 48 || _str[i] > 57)
        {
            return false;
        }
    }

    return true;
}

#endif // !TOOLS__HPP