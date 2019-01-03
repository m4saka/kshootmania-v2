#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "chart.h"
#include "beat_map/beat_map.h"
#include "chart_object/bt_note.h"
#include "chart_object/fx_note.h"
#include "chart_object/laser_note.h"

template <class Note>
using Lane = std::multimap<Measure, Note>;

// Chart (header & body)
class PlayableChart : public Chart
{
private:
    std::unique_ptr<BeatMap> m_beatMap;
    std::vector<Lane<BTNote> > m_btLanes;
    std::vector<Lane<FXNote> > m_fxLanes;
    std::vector<Lane<LaserNote> > m_laserLanes;

    bool insertTempoChange(std::map<Measure, double> & tempoChanges, Measure pos, std::string value);
    bool insertTimeSignatureChange(std::map<int, TimeSignature> & timeSignatureChanges, Measure pos, std::string value);

public:
    PlayableChart(const std::string & filename);
};
