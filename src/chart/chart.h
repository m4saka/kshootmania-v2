#pragma once

#include <string>

#include "chart_meta_data.h"
#include "chart_body.h"

class Chart
{
private:
    const std::string m_filename;
    const std::string m_fileDirectoryPath;

public:
    const ChartMetaData meta;
    const ChartBody body;
    Chart(const std::string & filename);
};
