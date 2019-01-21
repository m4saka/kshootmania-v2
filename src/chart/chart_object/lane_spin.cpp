#include "lane_spin.h"
#include "../beat_map/time_signature.h"

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
        // TODO: catch the exception from stoi
        length = std::stoi(strFromKsh.substr(2)) * UNIT_MEASURE / 192;
    }
}
