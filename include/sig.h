#pragma once

#include <iostream>
#include <vector>
#include <functional>

// Define a signal class
template <typename Payload>
class Signal
{
public:
    using Slot = std::function<void(const Payload &)>;

    // Method to connect a slot to the signal
    void connect(Slot slot)
    {
        slots.push_back(std::move(slot));
    }

    // Method to emit the signal with a payload
    void emit(const Payload &payload)
    {
        for (auto &slot : slots)
        {
            slot(payload);
        }
    }

private:
    std::vector<Slot> slots;
};