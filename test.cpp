
#include "cxxdclp.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    const cxxdclp& p = cxxdclp::parse("main.cpp");

    for (size_t i = 0; i < p.variables.size(); i++)
    {
        std::cout << p.variables[i].name << std::endl;
    }

    for (size_t i = 0; i < p.functions.size(); i++)
    {
        std::cout << p.functions[i].name << std::endl;
    }

    return 0;
}