#pragma once

#include "note.h"

struct BTNote final : public AbstractNote
{
private:

public:
    explicit BTNote(Measure length, Measure posForJudgmentAlignment = 0, bool halvesCombo = false)
        : AbstractNote(length)
    {
        if (length == 0)
        {
            m_judgments.emplace(0, NoteJudgment());
        }
        else
        {
            if (length <= oneJudgmentThreshold(halvesCombo))
            {
                m_judgments.emplace(0, NoteJudgment(length));
            }
            else
            {
                Measure interval = judgmentInterval(halvesCombo);
                Measure judgmentStart = ((posForJudgmentAlignment + interval - 1) / interval + 1) * interval - posForJudgmentAlignment;
                Measure judgmentEnd = length - interval;
                for (Measure pos = judgmentStart; pos < judgmentEnd; pos += interval)
                {
                    m_judgments.emplace(pos, NoteJudgment((pos > judgmentEnd - interval * 2) ? (judgmentEnd - pos) : interval));
                }
            }
        }
    }
};
