#pragma once

#include <string>
#include <vector>
#include <map>

#include "chart.h"
#include "beat_map/beat_map.h"
#include "chart_object/bt_note.h"
#include "chart_object/fx_note.h"
#include "chart_object/laser_note.h"

// Chart (header & body)
class PlayableChart : public Chart
{
private:
    template <class Note>
    using Lane = std::multimap<Measure, Note>

    const BeatMap m_beatMap;
    const std::vector<Lane<BTNote> > m_btLanes;
    const std::vector<Lane<FXNote> > m_fxLanes;
    const std::vector<Lane<LaserNote> > m_laserLanes;

public:
    PlayableChart(const std::string & filename);
};
