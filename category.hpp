#ifndef CATEGORY__HPP
#define CATEGORY__HPP

#include <vector>
#include <string>

class category
{
    private:
    std::vector<std::string> keywords;

    public:

    category(std::vector<std::string>&& _keywords) : keywords(_keywords) { }

    friend bool operator==(const std::string& _lhs, const category& _rhs)
    {
        for (size_t i = 0; i < _rhs.keywords.size(); i++)
        {
            if (_lhs == _rhs.keywords[i])
            {
                return true;
            }
        }

        return false;
    }

    static bool is_digit(const std::string& _str)
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
};

#endif // !CATEGORY__HPP