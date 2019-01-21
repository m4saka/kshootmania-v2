#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cstddef>

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
    std::vector<Lane<BTNote>> m_btLanes;
    std::vector<Lane<FXNote>> m_fxLanes;
    std::vector<Lane<LaserNote>> m_laserLanes;

    bool insertTempoChange(std::map<Measure, double> & tempoChanges, Measure pos, const std::string & value);

public:
    PlayableChart(const std::string & filename);

    const Lane<BTNote> & btLane(std::size_t idx) const
    {
        return m_btLanes.at(idx);
    }

    const Lane<FXNote> & fxLane(std::size_t idx) const
    {
        return m_fxLanes.at(idx);
    }

    const Lane<LaserNote> & laserLane(std::size_t idx) const
    {
        return m_laserLanes.at(idx);
    }

    std::size_t comboCount() const;
};
