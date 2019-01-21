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
    bool insertTempoChange(std::map<Measure, double> & tempoChanges, Measure pos, const std::string & value);

protected:
    std::unique_ptr<BeatMap> m_beatMap;
    std::vector<Lane<BTNote>> m_btLanes;
    std::vector<Lane<FXNote>> m_fxLanes;
    std::vector<Lane<LaserNote>> m_laserLanes;
    PlayableChart(const std::string & filename, bool isEditor);

public:
    PlayableChart(const std::string & filename) : PlayableChart(filename, false) {}

    virtual ~PlayableChart() = default;

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

    const std::vector<Lane<BTNote>> & btLanes() const
    {
        return m_btLanes;
    }

    const std::vector<Lane<FXNote>> & fxLanes() const
    {
        return m_fxLanes;
    }

    const std::vector<Lane<LaserNote>> & laserLanes() const
    {
        return m_laserLanes;
    }

    std::size_t comboCount() const;
};
