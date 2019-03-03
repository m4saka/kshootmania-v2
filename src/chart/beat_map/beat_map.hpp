#pragma once

#include <map>
#include <cstdint>

#include "time_signature.hpp"

// Millisecond
using Ms = double;

class BeatMap
{
private:
    const std::map<Measure, double> m_tempoChanges;
    const std::map<int, TimeSignature> m_timeSignatureChanges;
    mutable std::map<Measure, Ms> m_tempoChangeMsCache;
    mutable std::map<Ms, Measure> m_tempoChangeMeasureCache;
    mutable std::map<int, Measure> m_timeSignatureChangeMeasureCache;
    mutable std::map<Measure, int> m_timeSignatureChangeMeasureCountCache;

public:
    explicit BeatMap(double tempo) : BeatMap({ { 0, tempo } }) {}
    explicit BeatMap(const std::map<Measure, double> & tempoChanges = { { 0, 120.0 } },
        const std::map<int, TimeSignature> & timeSignatureChanges = { { 0, TimeSignature{ 4, 4 } } });
    Ms measureToMs(Measure measure) const;
    Measure msToMeasure(Ms ms) const;
    int measureToMeasureCount(Measure measure) const;
    int msToMeasureCount(Ms ms) const;
    Measure measureCountToMeasure(int measureCount) const;
    Measure measureCountToMeasure(double measureCount) const;
    Ms measureCountToMs(int measureCount) const;
    Ms measureCountToMs(double measureCount) const;
    bool isBarLine(Measure measure) const;
    double tempo(Measure measure) const;
    TimeSignature timeSignature(Measure measure) const;
};
