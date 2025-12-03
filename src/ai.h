#pragma once

#include <tuple>

#include "def.h"
#include "game.h"

class AI {
public:
    /** Performs a search to find the optimal move and returns it and the evaluation. */
    static std::tuple<u64, i32> find_move(Game game);

private:
    /** Alpha beta prune depth search. */
    static i32 score_move(Game game, u64 move, i32 depth, i32 a, i32 b);
};
