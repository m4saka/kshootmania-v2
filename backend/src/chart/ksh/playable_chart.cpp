#include "playable_chart.hpp"

#include <cstddef>
#include <cstdint>
#include <cassert>

#include "note_builder.hpp"

namespace ksh
{

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
        return line.find(OPTION_SEPARATOR) != std::string::npos;
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

    constexpr bool halvesCombo(double tempo)
    {
        return tempo >= 256.0;
    }

    std::string kshLegacyFXCharToAudioEffect(unsigned char c)
    {
        switch (c)
        {
        case 'S': return "Retrigger;8";
        case 'V': return "Retrigger;12";
        case 'T': return "Retrigger;16";
        case 'W': return "Retrigger;24";
        case 'U': return "Retrigger;32";
        case 'G': return "Gate;4";
        case 'H': return "Gate;8";
        case 'K': return "Gate;12";
        case 'I': return "Gate;16";
        case 'L': return "Gate;24";
        case 'J': return "Gate;32";
        case 'F': return "Flanger";
        case 'P': return "PitchShift";
        case 'B': return "BitCrusher";
        case 'Q': return "Phaser";
        case 'X': return "Wobble;12";
        case 'A': return "TapeStop";
        case 'D': return "SideChain";
        default:  return "";
        }
    }

    bool PlayableChart::insertTempoChange(std::map<Measure, double> & tempoChanges, Measure pos, const std::string & value)
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

    TimeSignature parseTimeSignature(std::string str)
    {
        std::size_t slashIdx = str.find('/');
        assert(slashIdx != std::string::npos);

        return TimeSignature{
            static_cast<uint32_t>(std::stoi(str.substr(0, slashIdx))),
            static_cast<uint32_t>(std::stoi(str.substr(slashIdx + 1)))
        };
    }

    PlayableChart::PlayableChart(const std::string & filename, bool isEditor)
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

        // FX audio effect string ("fx-l=" or "fx-r=" in .ksh)
        std::vector<std::string> currentFXAudioEffectStrs(m_fxLanes.size());

        // FX audio effect parameters ("fx-l_param1=" or "fx-r_param1=" in .ksh; currently no "param2")
        std::vector<std::string> currentFXAudioEffectParamStrs(m_fxLanes.size());

        // Insert the first tempo change
        double currentTempo = 120.0;
        if (metaData.count("t"))
        {
            if (insertTempoChange(tempoChanges, 0, metaData.at("t")))
            {
                currentTempo = tempoChanges.at(0);
            }
        }

        // Insert the first time signature change
        uint32_t currentNumerator = 4;
        uint32_t currentDenominator = 4;
        if (metaData.count("beat"))
        {
            TimeSignature timeSignature = parseTimeSignature(metaData.at("beat"));
            timeSignatureChanges.emplace(
                0,
                timeSignature
            );
            currentNumerator = timeSignature.numerator;
            currentDenominator = timeSignature.denominator;
        }
        else
        {
            timeSignatureChanges[0] = {4, 4};
        }

        // Buffers
        // (needed because actual addition cannot come before the measure value calculation)
        std::vector<std::string> chartLines;
        using OptionLine = std::pair<std::size_t, std::pair<std::string, std::string>>; // first = line index of chart lines
        std::vector<OptionLine> optionLines;

        Measure currentMeasure = 0;
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
                auto option = splitOptionLine(line);
                if (option.first == "t")
                {
                    if (option.second.find('-') == std::string::npos)
                    {
                        currentTempo = std::stod(option.second);
                    }
                    optionLines.emplace_back(chartLines.size(), option);
                }
                else if (option.first == "beat")
                {
                    TimeSignature timeSignature = parseTimeSignature(option.second);
                    timeSignatureChanges.emplace(
                        measureCount,
                        timeSignature
                    );
                    currentNumerator = timeSignature.numerator;
                    currentDenominator = timeSignature.denominator;
                }
                else if (option.first == "fx-l")
                {
                    currentFXAudioEffectStrs[0] = option.second;
                }
                else if (option.first == "fx-r")
                {
                    currentFXAudioEffectStrs[1] = option.second;
                }
                else if (option.first == "fx-l_param1")
                {
                    currentFXAudioEffectParamStrs[0] = option.second;
                }
                else if (option.first == "fx-r_param1")
                {
                    currentFXAudioEffectParamStrs[1] = option.second;
                }
                else
                {
                    optionLines.emplace_back(chartLines.size(), option);
                }
            }
            else if (isBarLine(line))
            {
                std::size_t resolution = chartLines.size();
                Measure linePosDiff = UNIT_MEASURE * currentNumerator / currentDenominator / resolution;

                // Add options that require their position
                for (auto && optionLine : optionLines)
                {
                    Measure pos = currentMeasure + linePosDiff * optionLine.first;
                    std::pair<std::string, std::string> & option = optionLine.second;
                    if (option.first == "t")
                    {
                        insertTempoChange(tempoChanges, pos, option.second);
                    }
                    else if (option.first == "zoom_top")
                    {
                        m_topLaneZooms.insert(pos, std::stod(option.second));
                    }
                    else if (option.first == "zoom_bottom")
                    {
                        m_bottomLaneZooms.insert(pos, std::stod(option.second));
                    }
                    else if (option.first == "zoom_side")
                    {
                        m_sideLaneZooms.insert(pos, std::stod(option.second));
                    }

                    // TODO: Store other options
                }

                // Add notes
                for (std::size_t i = 0; i < resolution; ++i)
                {
                    const std::string buf = chartLines.at(i);
                    std::size_t currentBlock = 0;
                    std::size_t laneCount = 0;

                    const Measure pos = currentMeasure + linePosDiff * i;

                    for (std::size_t j = 0; j < buf.size(); ++j)
                    {
                        if (buf[j] == BLOCK_SEPARATOR)
                        {
                            ++currentBlock;
                            laneCount = 0;
                            continue;
                        }

                        if (currentBlock == BLOCK_BT) // BT notes
                        {
                            assert(laneCount < btNoteBuilders.size());
                            switch (buf[j])
                            {
                            case '2': // Long BT note
                                btNoteBuilders[laneCount].prepareNote(pos, halvesCombo(currentTempo));
                                btNoteBuilders[laneCount].extendPreparedNoteLength(linePosDiff);
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
                                if (isEditor)
                                {
                                    const std::string audioEffectStr = (buf[j] == '1') ? currentFXAudioEffectStrs[laneCount] : kshLegacyFXCharToAudioEffect(buf[j]);
                                    fxNoteBuilders[laneCount].prepareNote(pos, halvesCombo(currentTempo), audioEffectStr, currentFXAudioEffectParamStrs[laneCount], true);
                                }
                                else
                                {
                                    fxNoteBuilders[laneCount].prepareNote(pos, halvesCombo(currentTempo));
                                    // TODO: Add audio effects independently for the game (because one FX note can have multiple audio effects)
                                }
                                fxNoteBuilders[laneCount].extendPreparedNoteLength(linePosDiff);
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
                                laserNoteBuilders[laneCount].extendPreparedNoteLength(linePosDiff);
                                break;
                            default:
                                const int laserX = LaserNote::charToLaserX(buf[j]);
                                if (laserX >= 0)
                                {
                                    laserNoteBuilders[laneCount].addPreparedNote(laserX);
                                    laserNoteBuilders[laneCount].prepareNote(pos, halvesCombo(currentTempo), laserX);
                                    laserNoteBuilders[laneCount].extendPreparedNoteLength(linePosDiff);
                                }
                            }
                        }
                        else if (currentBlock == BLOCK_LASER && laneCount == 2) // Lane spin
                        {
                            // Create a lane spin from string
                            const LaneSpin laneSpin(buf.substr(j));
                            if (laneSpin.type != LaneSpin::Type::NoSpin && laneSpin.direction != LaneSpin::Direction::Unspecified)
                            {
                                // Assign to the laser note builder if valid
                                const int laneIdx = (laneSpin.direction == LaneSpin::Direction::Left) ? 0 : 1;
                                laserNoteBuilders[laneIdx].prepareLaneSpin(laneSpin);
                            }
                        }
                        ++laneCount;
                    }
                }
                chartLines.clear();
                optionLines.clear();
                for (auto && str : currentFXAudioEffectStrs)
                {
                    str.clear();
                }
                for (auto && str : currentFXAudioEffectParamStrs)
                {
                    str.clear();
                }
                currentMeasure += UNIT_MEASURE * currentNumerator / currentDenominator;
                ++measureCount;
            }
        }

        m_ifs.close();

        m_beatMap = std::make_unique<BeatMap>(tempoChanges, timeSignatureChanges);
    }

    std::size_t PlayableChart::comboCount() const
    {
        std::size_t sum = 0;
        for (auto && lane : m_btLanes)
        {
            for (auto && pair : lane)
            {
                sum += pair.second.comboCount();
            }
        }
        for (auto && lane : m_fxLanes)
        {
            for (auto && pair : lane)
            {
                sum += pair.second.comboCount();
            }
        }
        for (auto && lane : m_laserLanes)
        {
            for (auto && pair : lane)
            {
                sum += pair.second.comboCount();
            }
        }
        return sum;
    }

}
