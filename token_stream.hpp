#ifndef TOKEN_STREAM__HPP
#define TOKEN_STREAM__HPP

#include <string>
#include <vector>

struct token_stream
{
    private:

    int recordedIndex = 0;
    std::vector<std::string> tokens;

    public:

    int index = 0;

    void clear() noexcept
    {
        index = 0;
        recordedIndex = 0;
        tokens.clear();
    }

    void push(const std::string& _token)
    {
        tokens.push_back(_token);
    }

    size_t size() const noexcept
    {
        return tokens.size();
    }

    bool finished() const noexcept
    {
        return index >= size();
    }

    const std::string& get() const
    {
        return tokens[index];
    }

    const std::string& back() const
    {
        return tokens.back();
    }

    const std::string& peek_next()
    {
        return tokens[index + 1];
    }

    const std::string& next()
    {
        return tokens[++index];
    }

    void record() noexcept
    {
        recordedIndex = index;
    }

    void rewind() noexcept
    {
        index = recordedIndex;
    }
};

#endif // !TOKEN_STREAM__HPP