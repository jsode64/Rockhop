#pragma once

#include <tuple>

#include "def.h"
#include "game.h"

class AI {
public:
    /** Searches to the given depth and returns the optimal move found and the evaluation. */
    static std::tuple<u64, i32> find_move(Game game, u32 depth);

private:
    /** Alpha beta prune depth search. */
    static i32 score_move(Game game, u64 move, u32 depth, i32 a, i32 b);
};
