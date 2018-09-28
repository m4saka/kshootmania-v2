#include <cstdint>

using Measure = int64_t;

struct AbstractChartObject
{
protected:
    explicit AbstractChartObject(Measure pos) : pos(pos) {}

public:
    Measure pos;
    virtual ~AbstractChartObject() = 0;
};
