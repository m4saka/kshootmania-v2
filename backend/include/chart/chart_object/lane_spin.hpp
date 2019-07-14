#pragma once

#include <string>

#include "chart/beat_map/time_signature.hpp"

struct LaneSpin
{
    enum class Type
    {
        NoSpin,
        Normal,
        Half,
        Swing,
    };
    Type type;

    enum class Direction
    {
        Unspecified,
        Left,
        Right,
    };
    Direction direction;

    Measure length;

    LaneSpin() : type(Type::NoSpin), direction(Direction::Unspecified), length(0) {}
    LaneSpin(Type type, Direction direction, Measure length) : type(type), direction(direction), length(length) {}
    explicit LaneSpin(std::string strFromKsh); // From .ksh spin string (example: "@(192")

    std::string toString() const; // To .ksh spin string (example: "@(192")
};
