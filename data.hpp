#ifndef DATA__HPP
#define DATA__HPP

#include <string>
#include <vector>

enum INSTRUCTION_TYPE
{
    UNKNOWN,
    NAMESPACE,
    CLASS,
    STRUCT,
    TYPEDEF,
    VARIABLE,
    FUNCTION
};

enum PROTECTION
{
    PUBLIC,
    PRIVATE,
    PROTECTED
};

struct common
{
    std::string name;
    std::vector<std::string> type;

    PROTECTION protection;
    
    bool isStatic;
};

struct variable : common
{
    std::vector<std::string> value;
};

struct function : common
{
    std::vector<variable> args;

    bool isPure;
    bool isVirtual;
};

struct type
{
    std::string name;
    std::vector<std::string> fullName;
    
    PROTECTION protection;

    bool isStruct;
    bool isTemplate;

    std::vector<std::string> templates;

    std::vector<variable> variables;
    std::vector<function> functions;
};

#endif // !DATA__HPP