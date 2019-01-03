#include "laser_note.h"

constexpr int LASER_X_MAX = 1000;

LaserNote::LaserNote(Measure length, double startX, double endX)
    : AbstractNote(length)
    , startX(startX)
    , endX(endX)
{
}

int LaserNote::charToLaserX(unsigned char c)
{
	if (c >= '0' && c <= '9')
    {
		return (c - '0') * LASER_X_MAX / 50;
	}
    else if (c >= 'A' && c <= 'Z')
    {
		return (c - 'A' + 10) * LASER_X_MAX / 50;
	}
    else if (c >= 'a' && c <= 'o')
    {
		return (c - 'a' + 36) * LASER_X_MAX / 50;
	}
    else
    {
        return -1;
    }
}