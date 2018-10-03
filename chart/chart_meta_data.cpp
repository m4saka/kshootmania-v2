#include "chart_meta_data.h"
#include <vector>
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

friend std::ostream & operator<<(std::ostream & os, const ChartMetaData & obj)
{
    // Output order-sensitive keys
    std::unordered_map<std::string, bool> finished;
    foreach (auto && key : {
        "title",
        "artist",
        "effect",
        "jacket",
        "illustrator",
        "difficulty",
        "level",
        "t",
        "m",
        "mvol",
        "o",
        "bg",
        "layer",
        "po",
        "plength",
    })
    {
        if (obj.count(key))
        {
            os << key << "=" << obj.at(key) << "\n";
            finished[key] = true;
        }
    }

    // Output remaining keys
    foreach (auto && param : obj.m_params)
    {
        if (finished.count(param.first) == 0)
        {
            os << param.first << "=" << param.second << "\n";
        }
    }

    os << std::flush;

    return os;
}
