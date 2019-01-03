#pragma once

#include "../beat_map/beat_map.h"

struct AbstractMusicalSegment
{
public:
    Measure length;
    explicit AbstractMusicalSegment(Measure length = 0) : length(length) {}
    virtual ~AbstractMusicalSegment() = default;
};
