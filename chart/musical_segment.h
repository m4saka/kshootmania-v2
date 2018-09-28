#include "beat_map.h"

struct AbstractMusicalSegment
{
public:
    Measure length;
	explicit MusicalNote(Measure length = 0) : length(length) {}
}
