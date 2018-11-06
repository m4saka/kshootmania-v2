#pragma once

#include <string>
#include <vector>
#include <map>

#include "beat_map/beat_map.h"
#include "chart_object/bt_note.h"
#include "chart_object/fx_note.h"
#include "chart_object/laser_note.h"

class ChartBody
{
private:
    const BeatMap m_beatMap;
    const std::vector<std::multimap<Measure, BTNote> > m_btLanes;
    const std::vector<std::multimap<Measure, FXNote> > m_fxLanes;
    const std::vector<std::multimap<Measure, LaserNote> > m_laserLanes;

public:
    ChartBody(const std::string & filename) {/* TODO: Loading chart */};
};
