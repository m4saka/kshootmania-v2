#pragma once

#include "musical_segment.hpp"

namespace ksh
{

    struct NoteJudgment : public AbstractMusicalSegment
    {
    public:
        enum class Result
        {
            Undefined,
            Error,
            Near,
            Critical,
        };
        Result result;

        NoteJudgment(Measure length = 0)
            : AbstractMusicalSegment(length)
            , result(Result::Undefined)
        {
        }
    };

}
