#include <cstdint>
#include <string>
#include <sstream>

using Measure = int64_t;

template <typename T>
struct AbstractChartOption
{
protected:
    AbstractChartOption(Measure pos, T value);

public:
    Measure pos;
    T value;
    virtual ~AbstractChartOption() = 0;
    std::string getValueString() const;
};

template <typename T>
AbstractChartOption<T>::AbstractChartOption(Measure pos, T value) :
    pos(pos),
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
