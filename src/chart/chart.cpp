#include "chart.h"
#include <fstream>

Chart::Chart(const std::string & filename)
    : m_filename(filename)
    , m_fileDirectoryPath(filename.substr(0, filename.find_last_of("/\\")))
    , meta(filename)
    , body(filename)
{
}
