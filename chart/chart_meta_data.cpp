#include "chart_meta_data.h"
#include <string>
#include <ifstream>

ChartMetaData(const std::string & filename)
{
    std::ifstream ifs(filename);
    std::string line;
    bool barLineExists = false;
    while (std::getline(ifs, line))
    {
        if (line == "--")
        {
            // Chart meta data is before first bar line ("--")
            barLineExists = true;
            break;
        }

        auto equalPos = line.find_first_of('=');
        if (equalPos == std::string::npos)
        {
            // The line doesn't have '='
            continue;
        }

        std::string key = line.substr(0, equalPos);
        std::string value = line.substr(equalPos);
        m_params[key] = value;
    }

    // .ksh files should have at least one bar line ("--")
    assert(barLineExists);
}