#include "musical_note.h"

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