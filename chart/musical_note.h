#include "beat_map.h"

struct AbstractMusicalNote
{
public:
    Measure length;
	explicit MusicalNote(Measure length = 0) : length(length) {}
}
