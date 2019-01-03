#include "note_builder.h"

AbstractNoteBuilder::AbstractNoteBuilder()
    : m_preparedNotePos(0)
    , m_preparedNoteLength(0)
    , m_notePrepared(false)
{
}

AbstractNoteBuilder::~AbstractNoteBuilder()
{
}

void AbstractNoteBuilder::resetPreparedNote()
{
    m_notePrepared = false;
}

void AbstractNoteBuilder::extendPreparedNoteLength(Measure diff)
{
    m_preparedNoteLength += diff;
}

BTNoteBuilder::BTNoteBuilder(Lane<BTNote> & lane)
    : m_lane(lane)
{
}

void BTNoteBuilder::addPreparedNote()
{
    if (m_notePrepared)
    {
        m_lane.emplace(m_preparedNotePos, BTNote(m_preparedNoteLength));
        m_notePrepared = false;
    }
}

void BTNoteBuilder::prepareNote(Measure pos, Measure length)
{
    if (!m_notePrepared)
    {
        m_notePrepared = true;
        m_preparedNotePos = pos;
        m_preparedNoteLength = length;
    }
}

FXNoteBuilder::FXNoteBuilder(Lane<FXNote> & lane)
    : m_lane(lane)
{
}

void FXNoteBuilder::addPreparedNote()
{
    if (m_notePrepared)
    {
        m_lane.emplace(m_preparedNotePos, FXNote(m_preparedNoteLength, m_preparedNoteAudioEffect));
        m_notePrepared = false;
    }
}

void FXNoteBuilder::prepareNote(Measure pos, Measure length)
{
    if (!m_notePrepared)
    {
        m_preparedNoteAudioEffect = ""; // Set long FX audio effect name to dummy
        m_notePrepared = true;
        m_preparedNotePos = pos;
        m_preparedNoteLength = length;
    }
}

void FXNoteBuilder::prepareNote(Measure pos, Measure length, const std::string & audioEffect)
{
    if (!m_notePrepared && audioEffect != m_preparedNoteAudioEffect)
    {
        addPreparedNote();
        m_preparedNoteAudioEffect = audioEffect;
        m_notePrepared = true;
        m_preparedNotePos = pos;
        m_preparedNoteLength = length;
    }
}

LaserNoteBuilder::LaserNoteBuilder(Lane<LaserNote> & lane)
    : m_lane(lane)
{
}

void LaserNoteBuilder::addPreparedNote(int preparedNoteLaserEndX)
{
    if (m_notePrepared)
    {
        m_lane.emplace(m_preparedNotePos, LaserNote(m_preparedNoteLength, m_preparedNoteLaserStartX, preparedNoteLaserEndX));
        m_notePrepared = false;
    }
}

void LaserNoteBuilder::prepareNote(Measure pos, Measure length, int laserStartX)
{
    m_notePrepared = true;
    m_preparedNotePos = pos;
    m_preparedNoteLength = length;
    m_preparedNoteLaserStartX = laserStartX;
}
