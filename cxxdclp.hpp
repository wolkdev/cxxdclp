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

        while (!tok.finished())
        {
            const std::string& token = tok.next();

            std::cout << token << "\n";
        }
    }
}

#endif // !CXXDCLP__HPP