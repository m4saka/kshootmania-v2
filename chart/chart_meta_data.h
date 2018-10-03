#pragma once

#include <string>
#include <cstddef>
#include <unordered_map>

struct ChartMetaData
{
private:
    std::unordered_map<std::string, std::string> m_params;

public:
    ChartMetaData(const std::string & filename);
};
