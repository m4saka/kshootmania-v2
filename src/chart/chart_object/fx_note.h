#pragma once

#include <string>

#include "note.h"

struct FXNote final : public AbstractNote
{
public:
    // Only used in editor
    std::string audioEffectStr;
    std::string audioEffectParamStr;

    explicit FXNote(Measure length, const std::string audioEffectStr = "", const std::string audioEffectParamStr = "", Measure posForJudgmentAlignment = 0, bool halvesCombo = false)
        : AbstractNote(length)
        , audioEffectStr(audioEffectStr)
        , audioEffectParamStr(audioEffectParamStr)
    {
        Measure judgmentStart = ((posForJudgmentAlignment + judgmentInterval(halvesCombo) - 1) / judgmentInterval(halvesCombo) + 1) * judgmentInterval(halvesCombo) - posForJudgmentAlignment;
        Measure judgmentEnd = length - judgmentInterval(halvesCombo);

        if (length == 0)
        {
            // Chip FX Note
            m_judgments.emplace(0, NoteJudgment());
        }
        else if (length <= oneJudgmentThreshold(halvesCombo))
        {
            // Long FX Note (too short to have multiple judgments)
            m_judgments.emplace(0, NoteJudgment(length));
        }
        else
        {
            // Long FX Note (long enough to have multiple judgments)
            Measure interval = judgmentInterval(halvesCombo);
            for (Measure pos = judgmentStart; pos < judgmentEnd; pos += interval)
            {
                m_judgments.emplace(pos, NoteJudgment((pos > judgmentEnd - interval * 2) ? (judgmentEnd - pos) : interval));
            }
        }
    }
};
