#pragma once

#include <map>

#include "../beat_map/beat_map.h"
#include "musical_segment.h"
#include "note_judgment.h"

struct AbstractNote : public AbstractMusicalSegment
{
protected:
    std::map<Measure, NoteJudgment> m_judgments;

public:
    explicit AbstractNote(Measure length) : AbstractMusicalSegment(length) {}
    virtual ~AbstractNote() = default;
};
