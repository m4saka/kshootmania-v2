#pragma once

#include "note.h"

struct LaserNote final : public AbstractNote
{
public:
    double startX;
    double endX;

    LaserNote(Measure length, double startX, double endX, Measure posForJudgmentAlignment = 0, bool halvesCombo = false);

    bool isSlam()
    {
        return length <= UNIT_MEASURE / 32;
    }

    static int charToLaserX(unsigned char c);
};
