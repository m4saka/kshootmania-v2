#pragma once

#include "musical_segment.h"

struct NoteJudgment : public AbstractMusicalSegment
{
public:
    enum class Result
    {
        ERROR,
        NEAR,
        CRITICAL,
    };
    Result result;
};