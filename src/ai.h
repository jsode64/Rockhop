#pragma once

#include <array>
#include <tuple>

#include "def.h"
#include "game.h"
#include "side.h"

class AI {
public:
    /** Searches to the given depth and returns the optimal move found and the evaluation. */
    static std::tuple<u64, i32> find_move(Game game, i32 depth);

private:
    /** Returns the legal moves sorted by instant potential in ascending order. */
    static std::tuple<std::array<u64, 6>, size> get_sorted_moves(Game game);

    /** Alpha beta prune depth search. */
    static i32 alpha_beta(Game game, u64 move, i32 depth, i32 a, i32 b);

    /** Scores the given move. */
    static i32 score_move(Side u, Side o, u64 i);
};
