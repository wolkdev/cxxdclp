#ifndef PARSING__HPP
#define PARSING__HPP

#include "data.hpp"
#include "token_stream.hpp"

#include <string>

bool is_native_type(const std::string& _str)
{
    return _str == "char"
        || _str == "int"
        || _str == "float"
        || _str == "unsigned"
        || _str == "short"
        || _str == "double"
        || _str == "long";
}

bool try_parse_native_type(token_stream& _i, std::string& _type)
{
    if (is_native_type(_i.get()))
    {
        _type = _i.get();

        while (is_native_type(_i.peek_next()))
        {
            _type += " " + _i.next();
        }

        return true;
    }

    return false;
}

bool try_parse_array(token_stream& _i, int& _arraySize)
{
    if (_i.get() == "[")
    {
        if (is_digit(_i.next()))
        {
            _arraySize = std::stoi(_i.get());
        }

        _i.index++; // skip ]
        
        return true;
    }

    return false;
}

bool try_parse_value(token_stream& _i, std::string& _value)
{
    if (_i.get() == "=")
    {
        int bracket = 0;

        while (_i.next() != ";" && _i.get() != "{"
            && (bracket || (_i.get() != "," && _i.get() != ")")))
        {
            if (_i.get() == "(")
            {
                bracket++;
            }
            else if (_i.get() == ")")
            {
                bracket--;
            }

            _value += _i.get();
        }

        _i.index--;

        return true;
    }

    return false;
}

bool try_parse_protection(token_stream& _i, PROTECTION& _protection)
{
    if (_i.get() == "public"
        && _i.peek_next() == ":")
    {
        _protection = PROTECTION::PUBLIC;
    }
    else if (_i.get() == "private"
        && _i.peek_next() == ":")
    {
        _protection = PROTECTION::PRIVATE;
    }
    else if (_i.get() == "protected"
        && _i.peek_next() == ":")
    {
        _protection = PROTECTION::PROTECTED;
    }
    else
    {
        return false;
    }

    _i.index++;

    return true;
}

bool try_parse_template(token_stream& _i)
{
    if (_i.get() == "template")
    {

    }
    else
    {
        return false;
    }

    return true;
}

bool try_parse_namespace(token_stream& _i, std::string& _namespace)
{
    if (_i.get() == "namespace")
    {
        _namespace = _i.next();

        return true;
    }

    return false;
}

bool try_parse_type(token_stream& _i, type& _type)
{
    if (_i.get() == "class")
    {
        _type.isClass = true;
        _type.name = _i.next();
    }
    else if (_i.get() == "struct")
    {
        _type.isStruct = true;
        _type.name = _i.next();
    }
    else
    {
        return false;
    }

    if (_i.back() == ";")
    {
        _type.forwardDcl = true;
    }

    return true;
}

bool try_parse_typedef(token_stream& _i, type& _type)
{
    if (_i.get() == "typedef")
    {

    }
    else
    {
        return false;
    }

    return true;
}

void parse_member(token_stream& _i, member& _member)
{
    int arraySize;
    bool typeNameLink = false;

    while (_i.get() != ";"
        && _i.get() != ","
        && _i.get() != "{")
    {
        if (_i.get() == ")")
        {
            if (!_member.type.isFunctionPtr && !_member.isFunction)
            {
                break;
            }
        }
        else if (_i.get() == "(")
        {
            if (_member.name.empty())
            {
                _member.type.isFunctionPtr = true;
            }
            else
            {
                _member.isFunction = !_member.type.isFunctionPtr;

                if (_i.next() != ")")
                {
                    do
                    {
                        _member.args.push_back(member());
                        parse_member(_i, _member.args.back());

                    } while (_i.get() == "," && ++_i.index < _i.size());
                }
            }
        }
        else if (_i.get() == "const")
        {
            _member.type.isConst = true;
        }
        else if (_i.get() == "::")
        {
            _member.type.name += _i.get();
            typeNameLink = true;
        }
        else if (_i.get() == "*")
        {
            _member.type.isPtr = true;
            _member.type.ptrDepth++;
        }
        else if (_i.get() == "&")
        {
            _member.type.isRef = true;
        }
        else if (_i.get() == "&&")
        {
            _member.type.isMoveRef = true;
        }
        else if (try_parse_array(_i, arraySize))
        {
            _member.arraySizes.push_back(arraySize);
        }
        else if (try_parse_value(_i, _member.value))
        {
            // do nothing else
        }
        else if (try_parse_native_type(_i, _member.type.name))
        {
            // do nothing else
        }
        else if (typeNameLink || _member.type.name.empty())
        {
            _member.type.name += _i.get();
            typeNameLink = false;
        }
        else
        {
            _member.name = _i.get();
        }

        _i.index++;
    }
}

#endif // !PARSING__HPP