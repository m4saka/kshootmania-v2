#pragma once

#include <iostream>
#include <string>
#include <cstddef>
#include <unordered_map>

class ChartMetaData
{
private:
    std::unordered_map<std::string, std::string> m_params;

public:
    ChartMetaData() = default;
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

    friend std::ostream & operator<<(std::ostream & os, const ChartMetaData & obj);

    std::string toString() const;
};
