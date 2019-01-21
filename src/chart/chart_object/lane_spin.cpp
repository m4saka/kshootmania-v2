#include "lane_spin.h"

#include "../beat_map/time_signature.h"

Measure kshLengthToMeasure(const std::string & str)
{
    // TODO: catch the exception from stoi
    return std::stoll(str) * UNIT_MEASURE / 192;
}

std::string measureToKshLength(Measure measure)
{
    return std::to_string(measure * 192 / UNIT_MEASURE);
}

LaneSpin::LaneSpin(std::string strFromKsh)
{
    // A .ksh spin string should have at least 3 chars
    if (strFromKsh.length() < 3)
    {
        type = Type::NoSpin;
        direction = Direction::Unspecified;
        length = 0;
        return;
    }

    // Specify the spin type
    if (strFromKsh[0] == '@')
    {
        switch (strFromKsh[1])
        {
        case '(':
            type = Type::Normal;
            direction = Direction::Left;
            break;

        case ')':
            type = Type::Normal;
            direction = Direction::Right;
            break;

        case '<':
            type = Type::Half;
            direction = Direction::Left;
            break;

        case '>':
            type = Type::Half;
            direction = Direction::Right;
            break;

        default:
            type = Type::NoSpin;
            direction = Direction::Unspecified;
            break;
        }
    }
    else if (strFromKsh[0] == 'S')
    {
        switch (strFromKsh[1])
        {
        case '<':
            type = Type::Swing;
            direction = Direction::Left;
            break;

        case '>':
            type = Type::Swing;
            direction = Direction::Right;
            break;

        default:
            type = Type::NoSpin;
            direction = Direction::Unspecified;
            break;
        }
    }
    else
    {
        type = Type::NoSpin;
        direction = Direction::Unspecified;
    }

    // Specify the spin length
    if (type == Type::NoSpin || direction == Direction::Unspecified)
    {
        length = 0;
    }
    else
    {
        length = kshLengthToMeasure(strFromKsh.substr(2));
    }
}

// TODO: Separate these ksh-specific methods
std::string LaneSpin::toString() const
{
    if (direction == Direction::Unspecified)
    {
        return "";
    }
    else
    {
        switch (type)
        {
        case Type::Normal:
            return std::string("@") + ((direction == Direction::Left) ? "(" : ")") + measureToKshLength(length);

        case Type::Half:
            return std::string("@") + ((direction == Direction::Left) ? "<" : ">") + measureToKshLength(length);

        case Type::Swing:
            return std::string("S") + ((direction == Direction::Left) ? "<" : ">") + measureToKshLength(length);

        default:
            return "";
        }
    }
}