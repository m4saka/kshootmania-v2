#pragma once

#include "../beat_map/beat_map.hpp"

namespace ksh
{

    struct AbstractMusicalSegment
    {
    public:
        Measure length;
        explicit AbstractMusicalSegment(Measure length = 0) : length(length) {}
        virtual ~AbstractMusicalSegment() = default;
    };

}
