#pragma once

enum class GaugeType
{
    EASY,
    NORMAL,
    HARD,
};

struct GameSettings
{
    bool isAutoPlay = false;
    GaugeType gaugeType = GaugeType::NORMAL;
};
