#include "chart_option.h"

class TempoChange : public AbstractChartOption<double>
{
public:
    TempoChange(Measure pos, double value) : AbstractChartOption<double>(pos, value) {}
};
