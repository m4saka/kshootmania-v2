#include "laser_note.hpp"

constexpr int LASER_X_MAX = 1000;

LaserNote::LaserNote(Measure length, double startX, double endX, Measure posForJudgmentAlignment, bool halvesCombo, const LaneSpin & laneSpin)
    : AbstractNote(length)
    , startX(startX)
    , endX(endX)
    , laneSpin(laneSpin)
{
    Measure judgmentStart = (posForJudgmentAlignment + judgmentInterval(halvesCombo) - 1) / judgmentInterval(halvesCombo) * judgmentInterval(halvesCombo) - posForJudgmentAlignment;
    Measure judgmentEnd = length;

    if (length <= UNIT_MEASURE / 32 && startX != endX) // Laser slam
    {
        m_judgments.emplace(0, NoteJudgment(length));
    }
    else
    {
        Measure interval = judgmentInterval(halvesCombo);
        for (Measure pos = judgmentStart; pos < judgmentEnd; pos += interval)
        {
            m_judgments.emplace(pos, NoteJudgment(interval));
        }
    }
}

int LaserNote::charToLaserX(unsigned char c)
{
    if (c >= '0' && c <= '9')
    {
        return (c - '0') * LASER_X_MAX / 50;
    }
    else if (c >= 'A' && c <= 'Z')
    {
        return (c - 'A' + 10) * LASER_X_MAX / 50;
    }
    else if (c >= 'a' && c <= 'o')
    {
        return (c - 'a' + 36) * LASER_X_MAX / 50;
    }
    else
    {
        return -1;
    }
}
