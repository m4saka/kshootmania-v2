#pragma once

#include <cstdint>
#include "beat_map.h"

struct TimeSignature
{
public:
    uint32_t numerator;
    uint32_t denominator;
    Measure measure() const
    {
        return UNIT_MEASURE * static_cast<Measure>(numerator) / static_cast<Measure>(denominator);
    }
};
