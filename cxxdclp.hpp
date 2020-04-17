#ifndef CXXDCLP__HPP
#define CXXDCLP__HPP

#include "externs/tokenizer/tokenizer.hpp"

#include "tools.hpp"

#include "data.hpp"

#include "parsing.hpp"

#include <cstdio>
#include <vector>
#include <string>

class cxxdclp
{
    private:

    static tokenizer stokenizer;

    private:
    
    std::string token;
    token_stream instruction;

    INSTRUCTION_TYPE instructionType;
    
    std::string pnamespace;
    type ptype;
    member pmember;

    PROTECTION protection;

    bool isStatic;
    bool isVirtual;
    bool isInline;
    bool isExtern;

    bool forwardDcl;

    public:

    std::vector<type> types;
    std::vector<variable> variables;
    std::vector<function> functions;

    private:

    void flush_instruction()
    {
        // TODO
    }

    void apply_instruction()
    {
        if (instructionType == INSTRUCTION_TYPE::NAMESPACE)
        {

        }
        else if (instructionType == INSTRUCTION_TYPE::CLASS
            || instructionType == INSTRUCTION_TYPE::STRUCT)
        {
            types.push_back(ptype);
        }
        else if (instructionType == INSTRUCTION_TYPE::VARIABLE)
        {
            variable var;
            var.name = pmember.name;
            var.value = pmember.value;
            var.type = pmember.type;
            var.protection = protection;
            var.isStatic = isStatic;

            variables.push_back(var);
        }
        else if (instructionType == INSTRUCTION_TYPE::FUNCTION)
        {
            function func;
            func.name = pmember.name;
            func.isPure = pmember.value == "0";
            func.type = pmember.type;
            func.protection = protection;
            func.isStatic = isStatic;
            
            for (size_t i = 0; i < pmember.args.size(); i++)
            {
                variable arg;
                arg.name = pmember.args[i].name;
                arg.value = pmember.args[i].value;
                arg.type = pmember.args[i].type;

                func.args.push_back(arg);
            }

            functions.push_back(func);
        }
    }

    void parse_instruction_token()
    {
        if (instruction.get() == "static")
        {
            isStatic = true;
        }
        else if (instruction.get() == "virtual")
        {
            isVirtual = true;
        }
        else if (instruction.get() == "inline")
        {
            isInline = true;
        }
        else if (instruction.get() == "extern")
        {
            isExtern = true;
        }
        else if (try_parse_protection(instruction, protection))
        {
            // do nothing else
        }
        else if (try_parse_template(instruction))
        {
            // do nothing else
        }
        else if (try_parse_namespace(instruction, pnamespace))
        {
            instructionType = INSTRUCTION_TYPE::NAMESPACE;
        }
        else if (try_parse_typedef(instruction, ptype))
        {
            instructionType = INSTRUCTION_TYPE::TYPEDEF;
        }
        else if (try_parse_type(instruction, ptype))
        {
            instructionType = ptype.isClass ?
                INSTRUCTION_TYPE::CLASS :
                INSTRUCTION_TYPE::STRUCT;
        }
        else
        {
            parse_member(instruction, pmember);
            
            instructionType = pmember.isFunction ?
                INSTRUCTION_TYPE::FUNCTION :
                INSTRUCTION_TYPE::VARIABLE;
        }
    }

    void parse_instruction()
    {
        while (!instruction.finished())
        {
            parse_instruction_token();
            instruction.index++;
        }
    }

    void get_nex_instruction()
    {
        bool preprocessor = false;

        instruction.clear();
        token.clear();

        while (!stokenizer.finished() &&
              (preprocessor ?
              (token != "\n") :
              (token != ";" && token != "}")))
        {
            token = stokenizer.next();

            if (token == "#")
            {
                preprocessor = true;
            }

            if (token == "//")
            {
                skip_until(stokenizer, "\n");
            }
            else if (token == "/*")
            {
                skip_until(stokenizer, "*/");
            }
            else
            {
                if (token != "\n")
                {
                    instruction.push(token);
                }

                if (token == "\"")
                {
                    instruction.push(get_string(stokenizer.text - 1));
                    stokenizer.text += instruction.back().size();
                }
            }
        }
    }

    void parse_file(const char* _filePath)
    {
        char* content = file_read_all_text(_filePath);

        if (content != nullptr)
        {
            stokenizer.start(content);

            while (!stokenizer.finished())
            {
                get_nex_instruction();
                parse_instruction();

                if (instructionType == INSTRUCTION_TYPE::FUNCTION
                    && instruction.back() == "{")
                {
                    skip_until(stokenizer, "}");
                }
            }
        }
    }

    public:
    static cxxdclp parse(const char* _startupPath)
    {
        cxxdclp parser;
        parser.parse_file(_startupPath);
        return parser;
    }
};

tokenizer cxxdclp::stokenizer = tokenizer
(
    " \t\r",                    // Dropped Delimiters

    std::vector<std::string>    // Kept Delimiters
    {
        "::",
        "&&",
        "->",
        "//",
        "/*",
        "*/",
        ";",
        "{",
        "}",
        "(",
        ")",
        ",",
        ":",
        "&",
        "*",
        "=",
        "+",
        "-",
        "/",
        "<",
        ">",
        "\"",
        "'",
        ".",
        "[",
        "]",
        "#",
        "\n"
    }
);

#endif // !CXXDCLP__HPP