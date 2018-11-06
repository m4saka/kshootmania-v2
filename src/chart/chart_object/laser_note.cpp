#include "laser_note.h"

LaserNote::LaserNote(Measure length, double start, double end)
    : AbstractNote(length)
    , start(start)
    , end(end)
{
}
