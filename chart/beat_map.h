#include <vector>

#include "tempo_change.h"
#include "time_signature_change.h"

class BeatMap
{
private:
    std::vector<TempoChange> m_tempoChanges;
    std::vector<TimeSignatureChange> m_timeSignatureChanges;
};
