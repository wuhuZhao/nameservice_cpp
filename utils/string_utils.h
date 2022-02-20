#include <string>
#include <vector>
namespace util
{
    std::vector<std::string> split(std::string &&s, char tag)
    {
        size_t front = 0;
        size_t end = 0;
        std::vector<std::string> ret;
        while (front <= end && end < s.size())
        {
            while (s[end] != tag && end < s.size())
                end++;
            std::string subStr = s.substr(front, end - front);
            if (subStr.size() != 0)
            {
                ret.push_back(subStr);
            }
            end++;
            front = end;
        }
        return ret;
    }
    std::vector<std::string> split(const std::string &s, char tag)
    {
                size_t front = 0;
        size_t end = 0;
        std::vector<std::string> ret;
        while (front <= end && end < s.size())
        {
            while (s[end] != tag && end < s.size())
                end++;
            std::string subStr = s.substr(front, end - front);
            if (subStr.size() != 0)
            {
                ret.push_back(subStr);
            }
            end++;
            front = end;
        }
        return ret;
    }
}