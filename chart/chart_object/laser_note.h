#pragma once

#include "note.h"

struct LaserNote final : public AbstractNote
{
public:
    double start;
    double end;
    LaserNote(Measure length, double start, double end);
};
