#pragma once

#include "chart/chart_object/abstract_note.hpp"

struct BTNote final : public AbstractNote
{
public:
    explicit BTNote(Measure length, Measure posForJudgmentAlignment = 0, bool halvesCombo = false)
        : AbstractNote(length)
    {
        if (length == 0)
        {
            // Chip BT Note
            m_judgments.emplace(0, NoteJudgment());
        }
        else if (length <= oneJudgmentThreshold(halvesCombo))
        {
            // Long BT Note (too short to have multiple judgments)
            m_judgments.emplace(0, NoteJudgment(length));
        }
        else
        {
            // Long BT Note (long enough to have multiple judgments)
            Measure interval = judgmentInterval(halvesCombo);
            Measure judgmentStart = ((posForJudgmentAlignment + interval - 1) / interval + 1) * interval - posForJudgmentAlignment;
            Measure judgmentEnd = length - interval;
            for (Measure pos = judgmentStart; pos < judgmentEnd; pos += interval)
            {
                m_judgments.emplace(pos, NoteJudgment((pos > judgmentEnd - interval * 2) ? (judgmentEnd - pos) : interval));
            }
        }
    }
};
