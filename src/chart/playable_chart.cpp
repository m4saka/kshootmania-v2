#include "playable_chart.h"

PlayableChart::PlayableChart(const std::string & filename)
    : Chart(filename, true) 
{
    using Chart::m_ifs;

    // TODO: parse chart body

    m_ifs.close();
}