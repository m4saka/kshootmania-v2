#pragma once

#include <vector>
#include <map>

#include "beat_map.h"
#include "bt_note.h"
#include "fx_note.h"
#include "laser_note.h"

class Chart
{
private:
    const std::string m_filename;
    const std::string m_fileDirectoryPath;
    const BeatMap m_beatMap;
    const std::vector<std::multimap<Measure, BTNote> > m_btLanes;
    const std::vector<std::multimap<Measure, FXNote> > m_fxLanes;
    const std::vector<std::multimap<Measure, LaserNote> > m_laserLanes;

public:
    Chart(const std::string & filename);
};
