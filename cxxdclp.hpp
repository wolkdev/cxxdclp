#ifndef CXXDCLP__HPP
#define CXXDCLP__HPP

#include "externs/tokenizer/tokenizer.hpp"

#include "tools.hpp"

#include "data.hpp"

#include <cstdio>
#include <vector>
#include <string>

class cxxdclp
{
    private:
    static tokenizer stokenizer;

    private:
    
    std::string token;
    std::vector<std::string> instruction;

    int index;

    std::string name;
    std::vector<std::string> strs_type;
    std::vector<std::string> strs_specifiers;
    PROTECTION protection;
    bool typelink;

    type ptype;
    member pmember;

    bool isStatic;
    bool isVirtual;
    bool isInline;
    bool isExtern;

    bool forwardDcl;

    public:
    std::vector<type> types;
    std::vector<variable> variables;
    std::vector<function> functions;
    std::vector<member> members;

    private:

    bool is_pointer()
    {
        return instruction[index] == "*";
    }

    bool is_reference()
    {
        return instruction[index] == "&";
    }

    bool is_move_reference()
    {
        return instruction[index] == "&&";
    }

    bool is_const()
    {
        return instruction[index] == "const";
    }

    bool is_type_link()
    {
        return instruction[index] == "::";
    }

    bool is_function_begin()
    {
        return instruction[index] == "(";
    }

    bool is_function_end()
    {
        return instruction[index] == ")";
    }

    bool is_comma()
    {
        return instruction[index] == ",";
    }

    bool is_native_type()
    {
        return instruction[index] == "char"
            || instruction[index] == "int"
            || instruction[index] == "float"
            || instruction[index] == "unsigned"
            || instruction[index] == "short"
            || instruction[index] == "double"
            || instruction[index] == "long";
    }

    bool try_parse_template()
    {
        if (instruction[index] == "template")
        {

        }
        else
        {
            return false;
        }
        
        return true;
    }

    bool try_parse_protection()
    {
        if (instruction[index] == "public"
            && instruction[++index] == ":")
        {
            protection = PROTECTION::PUBLIC;
        }
        else if (instruction[index] == "private"
            && instruction[++index] == ":")
        {
            protection = PROTECTION::PRIVATE;
        }
        else if (instruction[index] == "protected"
            && instruction[++index] == ":")
        {
            protection = PROTECTION::PROTECTED;
        }
        else
        {
            return false;
        }

        return true;
    }

    bool try_parse_specifier()
    {
        if (instruction[index] == "static")
        {
            isStatic = true;
        }
        else if (instruction[index] == "virtual")
        {
            isVirtual = true;
        }
        else if (instruction[index] == "inline")
        {
            isInline = true;
        }
        else if (instruction[index] == "extern")
        {
            isExtern = true;
        }
        else
        {
            return false;
        }
        
        return true;
    }

    bool try_parse_array(int& _arraySize)
    {
        if (instruction[index] == "["
            && is_digit(instruction[++index])
            && instruction[++index] == "]")
        {
            _arraySize = std::stoi(instruction[index - 1]);
        }
        else
        {
            return false;
        }

        return true;
    }

    bool try_parse_value(std::string& _value)
    {
        if (instruction[index] == "=")
        {
            int bracket = 0;

            while (++index < instruction.size()
                && instruction[index] != ";"
                &&
                (bracket
                || (instruction[index] != "," && instruction[index] != ")")))
            {
                if (instruction[index] == "(")
                {
                    bracket++;
                }
                else if (instruction[index] == ")")
                {
                    bracket--;
                }

                _value += instruction[index];
            }

            index--;
        }
        else
        {
            return false;
        }

        return true;
    }

    bool try_parse_type(type& _type)
    {
        if (instruction[index] == "class")
        {
            _type.isClass = true;
            _type.name = instruction[++index];
        }
        else if (instruction[index] == "struct")
        {
            _type.isStruct = true;
            _type.name = instruction[++index];
        }
        else if (instruction[index] == "typedef")
        {

        }
        else
        {
            return false;
        }

        if (instruction[instruction.size() - 1] == ";")
        {
            _type.forwardDcl = true;
        }

        return true;
    }

    void parse_member(member& _member)
    {
        int arraySize;
        bool typeNameLink = false;

        while (instruction[index] != ";"
            && instruction[index] != ","
            && instruction[index] != "{")
        {
            if (is_const())
            {
                if (_member.name.empty())
                {
                    _member.isConst = true;
                    _member.isConstRefOrPtr = true;
                }
                else
                {
                    _member.isConstRefOrPtr = true;
                }
            }
            else if (is_native_type())
            {
                if (!_member.typeName.empty())
                {
                    _member.typeName += " "; // Space
                }

                _member.typeName += instruction[index];
            }
            else if (is_type_link())
            {
                _member.typeName += instruction[index];
                typeNameLink = true;
            }
            else if (is_pointer())
            {
                _member.isPtr = true;
            }
            else if (is_reference())
            {
                _member.isRef = true;
            }
            else if (is_move_reference())
            {
                _member.isMoveRef = true;
            }
            else if (is_function_begin())
            {
                if (_member.name.empty())
                {
                    _member.isFunctionPtr = true;
                }
                else
                {
                    _member.isFunction = !_member.isFunctionPtr;

                    if (instruction[++index] != ")")
                    {
                        do
                        {
                            _member.args.push_back(member());
                            parse_member(_member.args.back());

                        } while (instruction[index] == ","
                            && ++index < instruction.size());
                    }
                }
            }
            else if (is_function_end())
            {
                if (!_member.isFunctionPtr && !_member.isFunction)
                {
                    break;
                }
            }
            else if (try_parse_array(arraySize))
            {
                _member.arraySizes.push_back(arraySize);
            }

            else if (try_parse_value(_member.value));

            else if (typeNameLink || _member.typeName.empty())
            {
                _member.typeName += instruction[index];
                typeNameLink = false;
            }
            else
            {
                _member.name = instruction[index];
            }

            index++;
        }
    }

    void parse_instruction_token()
    {
        if (try_parse_template())
        {

        }
        else if (try_parse_specifier())
        {

        }
        else if (try_parse_type(ptype))
        {
            types.push_back(ptype);
        }
        else
        {
            parse_member(pmember);
            members.push_back(std::move(pmember));
        }
        // ...
    }

    INSTRUCTION_TYPE parse_instruction()
    {
        for (index = 0; index < instruction.size(); index++)
        {
            parse_instruction_token();
        }
        
        return INSTRUCTION_TYPE::UNKNOWN;
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
                    instruction.push_back(token);
                }

                if (token == "\"")
                {
                    instruction.push_back(get_string(stokenizer.text - 1));
                    stokenizer.text += instruction.back().size();
                }
            }
        }
    }

    void parse_file(const char* _filePath)
    {
        char* content = file_read_all_text(_filePath);

        INSTRUCTION_TYPE type;

        if (content != nullptr)
        {
            stokenizer.start(content);

            while (!stokenizer.finished())
            {
                get_nex_instruction();
                type = parse_instruction();

                if (type == INSTRUCTION_TYPE::FUNCTION
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