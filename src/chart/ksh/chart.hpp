#pragma once

#include <string>
#include <unordered_map>
#include <fstream>

namespace ksh
{

    // Chart (header)
    class Chart
    {
    private:
        bool m_isUTF8;

    protected:
        const std::string m_filename;
        const std::string m_fileDirectoryPath;
        std::ifstream m_ifs;
        Chart(const std::string & filename, bool keepFileStreamOpen);

    public:
        // Chart meta data
        std::unordered_map<std::string, std::string> metaData;

        explicit Chart(const std::string & filename);

        virtual ~Chart() = default;

        std::string toString() const;

        bool isUTF8() const
        {
            return m_isUTF8;
        }
    };

}
