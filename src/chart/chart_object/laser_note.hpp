#pragma once

#include "note.hpp"
#include "lane_spin.hpp"

struct LaserNote final : public AbstractNote
{
public:
    double startX;
    double endX;
    LaneSpin laneSpin;

    LaserNote(Measure length, double startX, double endX, Measure posForJudgmentAlignment = 0, bool halvesCombo = false, const LaneSpin & laneSpin = LaneSpin());

    bool isSlam() const
    {
        return length <= UNIT_MEASURE / 32;
    }

    static int charToLaserX(unsigned char c);
};
