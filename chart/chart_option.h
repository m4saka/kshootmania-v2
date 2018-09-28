#include <string>
#include <sstream>
#include "chart_object.h"

template <typename T>
struct AbstractChartOption : public AbstractChartObject
{
protected:
    AbstractChartOption(Measure pos, T value);

public:
    T value;
    virtual ~AbstractChartOption() = 0;
    std::string getValueString() const;
};

template <typename T>
AbstractChartOption<T>::AbstractChartOption(Measure pos, T value) :
    AbstractChartObject(pos),
    value(value)
{
}

template <typename T>
std::string AbstractChartOption<T>::getValueString() const
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}
