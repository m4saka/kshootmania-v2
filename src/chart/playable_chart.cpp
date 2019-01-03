#include "playable_chart.h"

#include <cstddef>
#include <cstdint>
#include <cassert>

#include "note_builder.h"

constexpr unsigned char OPTION_SEPARATOR = '=';
constexpr unsigned char BLOCK_SEPARATOR = '|';
const std::string MEASURE_SEPARATOR = "--";

constexpr std::size_t BLOCK_BT = 0;
constexpr std::size_t BLOCK_FX = 1;
constexpr std::size_t BLOCK_LASER = 2;

bool isChartLine(const std::string & line)
{
    return line.find(BLOCK_SEPARATOR) != std::string::npos;
}

bool isOptionLine(const std::string & line)
{
    return line.find(BLOCK_SEPARATOR) != std::string::npos;
}

bool isBarLine(const std::string & line)
{
    return line == MEASURE_SEPARATOR;
}

std::pair<std::string, std::string> splitOptionLine(const std::string optionLine)
{
    std::size_t equalIdx = optionLine.find_first_of(OPTION_SEPARATOR);

    // Option line should have "="
    assert(equalIdx != std::string::npos);

    return std::pair<std::string, std::string>(
        optionLine.substr(0, equalIdx),
        optionLine.substr(equalIdx + 1)
    );
}

bool PlayableChart::insertTempoChange(std::map<Measure, double> & tempoChanges, Measure pos, std::string value)
{
    if (value.find('-') == std::string::npos)
    {
        tempoChanges.emplace(pos, std::stod(value));
        return true;
    }
    else
    {
        return false;
    }
}

bool PlayableChart::insertTimeSignatureChange(std::map<int, TimeSignature> & timeSignatureChanges, Measure pos, std::string value)
{
    std::size_t slashIdx = value.find('/');
    if (slashIdx != std::string::npos)
    {
        timeSignatureChanges.emplace(
            0,
            TimeSignature{
                static_cast<uint32_t>(std::stoi(value.substr(0, slashIdx))),
                static_cast<uint32_t>(std::stoi(value.substr(slashIdx + 1)))
            }
        );
        return true;
    }
    else
    {
        return false;
    }
}

PlayableChart::PlayableChart(const std::string & filename)
    : Chart(filename, true)
    , m_btLanes(4)
    , m_fxLanes(2)
    , m_laserLanes(2)
{
    // TODO: Catch exceptions from std::stod()

    std::map<Measure, double> tempoChanges;
    std::map<int, TimeSignature> timeSignatureChanges;

    // Note builders for note insertion to lanes
    std::vector<BTNoteBuilder> btNoteBuilders;
    for (auto && lane : m_btLanes)
    {
        btNoteBuilders.emplace_back(lane);
    }
    std::vector<FXNoteBuilder> fxNoteBuilders;
    for (auto && lane : m_fxLanes)
    {
        fxNoteBuilders.emplace_back(lane);
    }
    std::vector<LaserNoteBuilder> laserNoteBuilders;
    for (auto && lane : m_laserLanes)
    {
        laserNoteBuilders.emplace_back(lane);
    }

    // Insert the first tempo change
    if (metaData.count("t"))
    {
        insertTempoChange(tempoChanges, 0, metaData.at("t"));
    }

    // Insert the first time signature change
    if (metaData.count("beat"))
    {
        insertTimeSignatureChange(timeSignatureChanges, 0, metaData.at("beat"));
    }

    // Buffers
    // (needed because actual addition cannot come before the measure value calculation)
    std::vector<std::string> chartLines;
    using OptionLine = std::pair<std::size_t, std::string>; // first = line index of chart lines
    std::vector<OptionLine> optionLines;

    std::size_t measureCount = 0;

    // Read chart body
    // Expect m_ifs to start from the next of the first bar line ("--")
    std::string line;
    while (std::getline(m_ifs, line, '\n'))
    {
        // Eliminate CR
        if(!line.empty() && *line.crbegin() == '\r') {
            line.pop_back();
        }

        // Skip comments
        if (line[0] == ';' || line.substr(0, 2) == "//")
        {
            continue;
        }

        // TODO: Read user-defined audio effects
        if (line[0] == '#')
        {
            continue;
        }

        if (isChartLine(line))
        {
            chartLines.push_back(line);
        }
        else if (isOptionLine(line))
        {
            optionLines.emplace_back(chartLines.size(), line);
        }
        else if (isBarLine(line))
        {
            std::size_t resolution = chartLines.size();

            // Add options
            for (auto && optionLine : optionLines)
            {
                Measure pos = UNIT_MEASURE * measureCount + UNIT_MEASURE * optionLine.first / resolution;
                auto option = splitOptionLine(optionLine.second);
                if (option.first == "t")
                {
                    insertTempoChange(tempoChanges, pos, option.second);
                }
                else if (option.first == "beat")
                {
                    insertTimeSignatureChange(timeSignatureChanges, pos, option.second);
                }

                // TODO: Store other options
            }

            // Add notes
            for (std::size_t i = 0; i < resolution; ++i)
            {
                std::string buf = chartLines.at(i);
                std::size_t currentBlock = 0;
                std::size_t laneCount = 0;
                for (std::size_t j = 0; j < buf.size(); ++j)
                {
                    if (buf[j] == BLOCK_SEPARATOR)
                    {
                        ++currentBlock;
                        laneCount = 0;
                        continue;
                    }
                    
                    Measure pos = UNIT_MEASURE * measureCount + UNIT_MEASURE * i / resolution;

                    if (currentBlock == BLOCK_BT) // BT notes
                    {
                        assert(laneCount < btNoteBuilders.size());
                        switch (buf[j])
                        {
                        case '2': // Long BT note
                            btNoteBuilders[laneCount].prepareNote(pos, UNIT_MEASURE / resolution);
                            break;
                        case '1': // Chip BT note
                            m_btLanes[laneCount].emplace(pos, BTNote(0));
                            break;
                        default:  // Empty
                            btNoteBuilders[laneCount].addPreparedNote();
                        }
                    }
                    else if (currentBlock == BLOCK_FX) // FX notes
                    {
                        assert(laneCount < fxNoteBuilders.size());
                        switch (buf[j])
                        {
                        case '2': // Chip FX note
                            m_fxLanes[laneCount].emplace(pos, FXNote(0));
                            break;
                        case '0': // Empty
                            fxNoteBuilders[laneCount].addPreparedNote();
                            break;
                        default:  // Long FX note
                            fxNoteBuilders[laneCount].prepareNote(pos, UNIT_MEASURE / resolution);
                        }
                    }
                    else if (currentBlock == BLOCK_LASER && laneCount < 2) // Laser notes
                    {
                        assert(laneCount < laserNoteBuilders.size());
                        switch (buf[j])
                        {
                        case '-': // Empty
                            laserNoteBuilders[laneCount].resetPreparedNote();
                            break;
                        case ':': // Connection
                            laserNoteBuilders[laneCount].extendPreparedNoteLength(UNIT_MEASURE / resolution);
                            break;
                        default:
                            int laserX = LaserNote::charToLaserX(buf[j]);
                            if (laserX >= 0)
                            {
                                laserNoteBuilders[laneCount].addPreparedNote(laserX);
                                laserNoteBuilders[laneCount].prepareNote(pos, UNIT_MEASURE / resolution, laserX);
                            }
                        }
                    }
                    ++laneCount;
                }
            }
            chartLines.clear();
        }
    }

    m_ifs.close();
}