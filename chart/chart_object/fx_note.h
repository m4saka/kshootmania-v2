#pragma once

#include "note.h"

struct FXNote final : public AbstractNote
{
public:
    explicit FXNote(Measure length = 0) : AbstractNote(length) {}
};
