#pragma once

#include <string>
#include <cstddef>

#include "playable_chart.h"
#include "chart_object/bt_note.h"
#include "chart_object/fx_note.h"
#include "chart_object/laser_note.h"

// Temporary class for inserting notes to the lane
class AbstractNoteBuilder
{
protected:
    Measure m_preparedNotePos;
    Measure m_preparedNoteLength;
    bool m_notePrepared;

public:
    explicit AbstractNoteBuilder();

    void resetPreparedNote();

    void extendPreparedNoteLength(Measure diff);

    virtual ~AbstractNoteBuilder() = 0;
};

class BTNoteBuilder : public AbstractNoteBuilder
{
private:
    Lane<BTNote> & m_lane;

public:
    explicit BTNoteBuilder(Lane<BTNote> & lane);

    void addPreparedNote();

    // Prepare a long BT note
    void prepareNote(Measure pos, Measure length);
};

class FXNoteBuilder : public AbstractNoteBuilder
{
private:
    Lane<FXNote> & m_lane;
    std::string m_preparedNoteAudioEffect;

public:
    explicit FXNoteBuilder(Lane<FXNote> & lane);

    void addPreparedNote();

    // Prepare a long FX note
    void prepareNote(Measure pos, Measure length);

    // Prepare a long FX note (for editor; split the note if audio effects are different)
    void prepareNote(Measure pos, Measure length, const std::string & audioEffect);
};

class LaserNoteBuilder : public AbstractNoteBuilder
{
private:
    Lane<LaserNote> & m_lane;
    int m_preparedNoteLaserStartX;

public:
    explicit LaserNoteBuilder(Lane<LaserNote> & lane);

    void addPreparedNote(int preparedNoteLaserEndX);

    // Prepare a laser note
    void prepareNote(Measure pos, Measure length, int laserStartX);
};
