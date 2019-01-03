#pragma once

#include "note.h"

struct FXNote final : public AbstractNote
{
public:
    explicit FXNote(Measure length, const std::string & audioEffect = "")
        : AbstractNote(length)
    {
    }
};
