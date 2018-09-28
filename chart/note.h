#include <map>

#include "beat_map.h"
#include "musical_note.h"
#include "note_judgment.h"

struct AbstractNote : public AbstractMusicalSegment
{
protected:
    std::map<Measure, NoteJudgment> m_judgments;

public:
    explicit AbstractNote(Measure length = 0) : AbstractMusicalSegment(length) {}
}
