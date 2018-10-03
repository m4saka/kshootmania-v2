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

    std::size_t size() const
    {
        return m_params.size();
    }

    std::size_t count(const std::string & key) const
    {
        return m_params.count(key);
    }

    const std::string & at(const std::string & key) const
    {
        return m_params.at(key);
    }

    std::string & operator[](const std::string & key)
    {
        return m_params[key];
    }
};
