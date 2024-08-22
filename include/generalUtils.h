#pragma once

#include <vector>
#include <string>
#include <algorithm>

bool doesVectorContainString(const std::vector<std::string> &vec, const std::string &str);

template <typename T>
T getRandomItemFromVector(const std::vector<T> &vec)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    int randomIndex = std::rand() % vec.size();
    return vec[randomIndex];
}
