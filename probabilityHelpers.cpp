#include "include/probabilityHelpers.h"

bool calculateChance(int percentChance)
{
    // Create a random device to seed the random number generator
    std::random_device rd;
    // Use the Mersenne Twister engine for random number generation
    std::mt19937 gen(rd());
    // Define a uniform distribution in the range [0, 99]
    std::uniform_int_distribution<> dis(0, 99);

    // Generate a random number and check if it's less than the given percentage
    if (dis(gen) < percentChance)
    {
        return true;
    }
    return false;
}