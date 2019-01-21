#pragma once

#include "playable_chart.h"

// Chart (header & body)
class EditableChart : public PlayableChart
{
public:
    EditableChart(const std::string & filename) : PlayableChart(filename, true) {}
}
