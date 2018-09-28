#include "musical_note.h"

struct NoteJudgment : public AbstractMusicalNote
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