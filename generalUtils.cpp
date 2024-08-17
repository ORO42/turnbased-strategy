#include "include/generalUtils.h"

bool doesVectorContainString(const std::vector<std::string> &vec, const std::string &str)
{
    return std::find(vec.begin(), vec.end(), str) != vec.end();
}