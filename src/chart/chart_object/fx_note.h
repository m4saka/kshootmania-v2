#pragma once

#include <string>

#include "note.h"

struct FXNote final : public AbstractNote
{
public:
    explicit FXNote(Measure length, const std::string audioEffect = "", Measure posForJudgmentAlignment = 0, bool halvesCombo = false)
        : AbstractNote(length)
    {
        Measure judgmentStart = ((posForJudgmentAlignment + judgmentInterval(halvesCombo) - 1) / judgmentInterval(halvesCombo) + 1) * judgmentInterval(halvesCombo) - posForJudgmentAlignment;
        Measure judgmentEnd = length - judgmentInterval(halvesCombo);

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
                for (Measure pos = judgmentStart; pos < judgmentEnd; pos += interval)
                {
                    m_judgments.emplace(pos, NoteJudgment((pos > judgmentEnd - interval * 2) ? (judgmentEnd - pos) : interval));
                }
            }
        }
    }
};
