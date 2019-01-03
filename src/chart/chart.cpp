#include "chart.h"

#include <sstream>
#include <cassert>

Chart::Chart(const std::string & filename, bool keepFileStreamOpen)
    : m_filename(filename)
    , m_fileDirectoryPath(filename.substr(0, filename.find_last_of("/\\")))
{
    m_ifs.open(filename, std::ifstream::in); // TODO: Error handling

    // Eliminate UTF-8 BOM
    std::string firstLine;
    std::getline(m_ifs, firstLine, '\n');
    if (firstLine.length() >= 3 && firstLine.substr(0, 3) == "\xEF\xBB\xBF")
    {
        m_isUTF8 = true;
        m_ifs.seekg(3, std::ios_base::beg);
    }
    else
    {
        m_isUTF8 = false;
        m_ifs.seekg(0, std::ios_base::beg);
    }

    std::string line;
    bool barLineExists = false;
    while (std::getline(m_ifs, line, '\n'))
    {
        // Eliminate CR
        if(!line.empty() && *line.crbegin() == '\r') {
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
        metaData[key] = value;
    }

    // .ksh files should have at least one bar line ("--")
    assert(barLineExists);

    if (!keepFileStreamOpen)
    {
        m_ifs.close();
    }
}

Chart::Chart(const std::string & filename) : Chart(filename, false)
{
}

std::string Chart::toString() const
{
    std::ostringstream oss;

    // Output order-sensitive keys
    std::unordered_map<std::string, bool> finished;
    for (auto && key : {
        "title",
        "title_img",
        "artist",
        "artist_img",
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
        if (metaData.count(key))
        {
            oss << key << "=" << metaData.at(key) << "\r\n";
            finished[key] = true;
        }
    }

    // Output remaining keys
    for (auto && param : metaData)
    {
        if (finished.count(param.first) == 0)
        {
            oss << param.first << "=" << param.second << "\r\n";
        }
    }

    oss << std::flush;

    return oss.str();
}