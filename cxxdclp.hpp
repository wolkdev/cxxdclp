#ifndef CXXDCLP__HPP
#define CXXDCLP__HPP

#include "externs/tokenizer/tokenizer.hpp"

#include <cstdio>
#include <iostream>

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

void skip_until(tokenizer& _tok, const std::string& _str)
{
    while (!_tok.finished() && _tok.next() != _str) { }
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

void parse_line(const std::vector<std::string>& _tokens)
{
    size_t size = _tokens.size();

    size_t equals = find_token_pos(_tokens, "=");
    size_t bracket = find_token_pos(_tokens, "(");

    if (equals != std::string::npos)
    {
        // pure function
        if (bracket != std::string::npos
            && bracket < equals
            && _tokens[equals + 1] == "0")
        {

        }
        // variable
        else
        {

        }
    }
}

void parse(const char* _filePath)
{
    char* content = file_read_all_text(_filePath);

    if (content != nullptr)
    {
        tokenizer tok;

        tok.set_dropped_delimiters(" \t\r\n");

        tok.add_kept_delimiter("template");
        tok.add_kept_delimiter("struct");
        tok.add_kept_delimiter("class");
        tok.add_kept_delimiter("enum");
        tok.add_kept_delimiter("typedef");
        tok.add_kept_delimiter("typename");

        tok.add_kept_delimiter("mutable");
        tok.add_kept_delimiter("default");

        tok.add_kept_delimiter("const");
        tok.add_kept_delimiter("if");
        tok.add_kept_delimiter("else");
        tok.add_kept_delimiter("for");
        tok.add_kept_delimiter("while");
        tok.add_kept_delimiter("do");
        tok.add_kept_delimiter("public");
        tok.add_kept_delimiter("protected");
        tok.add_kept_delimiter("private");
        tok.add_kept_delimiter("nullptr");
        tok.add_kept_delimiter("auto");
        tok.add_kept_delimiter("new");
        tok.add_kept_delimiter("delete");
        tok.add_kept_delimiter("static");
        tok.add_kept_delimiter("virtual");
        tok.add_kept_delimiter("override");

        tok.add_kept_delimiter("include");
        tok.add_kept_delimiter("define");
        tok.add_kept_delimiter("ifndef");
        tok.add_kept_delimiter("ifdef");
        tok.add_kept_delimiter("endif");
        tok.add_kept_delimiter("elif");
        tok.add_kept_delimiter("undef");

        tok.add_kept_delimiter("unsigned");
        tok.add_kept_delimiter("void");
        tok.add_kept_delimiter("int");
        tok.add_kept_delimiter("float");
        tok.add_kept_delimiter("double");
        tok.add_kept_delimiter("char");

        tok.add_kept_delimiter(";");
        tok.add_kept_delimiter("{");
        tok.add_kept_delimiter("}");

        tok.add_kept_delimiter("::");
        tok.add_kept_delimiter("&&");
        tok.add_kept_delimiter("->");
        tok.add_kept_delimiter("//");
        tok.add_kept_delimiter("/*");
        tok.add_kept_delimiter("*/");
        tok.add_kept_delimiter("(");
        tok.add_kept_delimiter(")");
        tok.add_kept_delimiter(",");
        tok.add_kept_delimiter(":");
        tok.add_kept_delimiter("&");
        tok.add_kept_delimiter("*");
        tok.add_kept_delimiter("=");
        tok.add_kept_delimiter("+");
        tok.add_kept_delimiter("-");
        tok.add_kept_delimiter("/");
        tok.add_kept_delimiter("<");
        tok.add_kept_delimiter(">");
        tok.add_kept_delimiter("\"");
        tok.add_kept_delimiter("'");
        tok.add_kept_delimiter(".");
        tok.add_kept_delimiter("[");
        tok.add_kept_delimiter("]");
        tok.add_kept_delimiter("#");
        
        tok.start(content);

        std::vector<std::string> lineTokens;

        while (!tok.finished())
        {
            const std::string& token = tok.next();

            if (token == ";" || token == "{")
            {
                if (token == "{")
                {
                    skip_until(tok, "}");
                }
            }
            else
            {
                lineTokens.push_back(token);
            }

            std::cout << token << "\n";
        }
    }
}

#endif // !CXXDCLP__HPP