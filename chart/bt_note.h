#pragma once

#include "note.h"

struct BTNote final : public AbstractNote
{
public:
    explicit BTNote(Measure length = 0) : AbstractNote(length) {}
};
