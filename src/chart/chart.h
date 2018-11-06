#pragma once

#include <vector>
#include <map>

#include "beat_map/beat_map.h"
#include "note/bt_note.h"
#include "note/fx_note.h"
#include "note/laser_note.h"

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
