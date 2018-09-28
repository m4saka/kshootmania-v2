#include "chart_option.h"
#include "time_signature.h"

class TimeSignatureChange : public AbstractChartOption<TimeSignature>
{
public:
    TimeSignatureChange(Measure pos, TimeSignature value) : AbstractChartOption<TimeSignature>(pos, value) {}
};
