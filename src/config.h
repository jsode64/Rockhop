#pragma once

#include "def.h"

/** Command line interface. */
namespace ci {
    /** The default search depth for "go". */
    constexpr u32 GO_DEFAULT_DEPTH      = 20;

    /** The default search depth for "eval". */
    constexpr u32 EVAL_DEFAULT_DEPTH    = 22;
}

/** Evaluation weights. */
namespace ew {
    /** The evaluation weight/score of having a guaranteed winning position. */
    constexpr i32 WINNING               = 2'000'000;

    /** The evaluation weight of a stone in the mancala. */
    constexpr i32 STONE_IN_MANCALA      = 50;

    /** The evaluation weight of a stone in a pit. */
    constexpr i32 STONE_IN_PIT          = 5;

    /** The evaluation weight of a stone gained in a capture. */
    constexpr i32 STONE_FROM_CAPTURE    = 55;

    /** The evaluation weight of it being own turn. */
    constexpr i32 OWN_TURN              = 20;
}

/** Rules. */
namespace rl {
    /** Number of pits in each side (not counting the mancala). */
    constexpr u64 N_PITS                = 6;

    /** Number of stones in each pit at the start of the game. */
    constexpr u64 N_STARTING_STONES     = 4;

    /** This is to prevent overflow in the pits since each is 8 bits. */
    static_assert(N_STARTING_STONES < 256 / 12);

    /** The function `Side::make_move` requires this to work at all. */
    static_assert(N_PITS == 6);
}
