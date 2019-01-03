#pragma once

#include <map>
#include <cstddef>

#include "../beat_map/beat_map.h"
#include "musical_segment.h"
#include "note_judgment.h"

struct AbstractNote : public AbstractMusicalSegment
{
protected:
    std::map<Measure, NoteJudgment> m_judgments;

public:
    explicit AbstractNote(Measure length = 0)
        : AbstractMusicalSegment(length)
    {
    }

    virtual ~AbstractNote() = default;

    std::size_t comboCount() const
    {
        return m_judgments.size();
    }

    static constexpr Measure judgmentInterval(bool halvesCombo)
    {
        return UNIT_MEASURE / (halvesCombo ? 8 : 16);
    }

    static constexpr Measure oneJudgmentThreshold(bool halvesCombo)
    {
        return UNIT_MEASURE * 3 / (halvesCombo ? 8 : 16);
    }
};
