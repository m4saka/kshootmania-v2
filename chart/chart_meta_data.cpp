#include "chart_meta_data.h"
#include <cassert>
#include <vector>
#include <fstream>
#include <sstream>

ChartMetaData::ChartMetaData(const std::string & filename)
{
    std::ifstream ifs(filename);

    // Eliminate UTF-8 BOM
    // TODO: Convert Shift-JIS (or other encodes) to UTF-8 if BOM doesn't exist
    ifs.seekg(3, std::ios_base::beg);

    std::string line;
    bool barLineExists = false;
    while (std::getline(ifs, line, '\n'))
    {
        // Eliminate CR
        if(!line.empty() && *line.rbegin() == '\r') {
            line.pop_back();
        }

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
        std::string value = line.substr(equalPos + 1);
        m_params[key] = value;
    }

    // .ksh files should have at least one bar line ("--")
    assert(barLineExists);
}

std::ostream & operator<<(std::ostream & os, const ChartMetaData & obj)
{
    // Output order-sensitive keys
    std::unordered_map<std::string, bool> finished;
    for (auto && key : {
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
            os << key << "=" << obj.at(key) << "\r\n";
            finished[key] = true;
        }
    }

    // Output remaining keys
    for (auto && param : obj.m_params)
    {
        if (finished.count(param.first) == 0)
        {
            os << param.first << "=" << param.second << "\r\n";
        }
    }

    os << std::flush;

    return os;
}

std::string ChartMetaData::toString() const
{
    std::ostringstream oss;
    oss << *this;
    return oss.str();
}
