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
    FUNCTION,
    CONTEXT_END
};

enum PROTECTION
{
    PUBLIC,
    PRIVATE,
    PROTECTED
};

struct type_definition
{

};

struct type_instance
{
    std::string name;
    std::vector<std::string> fullName;

    bool isConst = false;

    bool isPtr = false;
    bool isRef = false;
    bool isMoveRef = false;

    int ptrDepth = 0;

    bool isFunctionPtr = false;

    std::vector<int> arraySizes;
};

struct common
{
    std::string name;
    std::vector<std::string> fullName;
    
    type_instance type;

    PROTECTION protection;
    
    bool isStatic;
};

struct variable : common
{
    std::string value;
};

struct function : common
{
    std::vector<variable> args;

    bool isPure;
    bool isVirtual;
};

struct member
{
    std::string name;
    type_instance type;
    std::string value;
    std::vector<member> args;

    std::vector<int> arraySizes;

    bool isFunction = false;
    bool isPure = false;
};

struct type
{
    std::string name;
    std::vector<std::string> fullName;
    
    PROTECTION protection;

    bool isClass = false;
    bool isStruct = false;
    bool isTemplate = false;

    bool forwardDcl = false;

    std::vector<std::string> templates;

    std::vector<variable> variables;
    std::vector<function> functions;
};

#endif // !DATA__HPP