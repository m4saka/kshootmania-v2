#pragma once

#include "musical_segment.h"

struct NoteJudgment : public AbstractMusicalSegment
{
public:
    enum class Result
    {
        Error,
        Near,
        Critical,
    };
    Result result;
};