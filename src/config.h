#pragma once

#include "def.h"

/**
 * @brief The number of stones starting in each pit.
 */
static constexpr inline u64 N_STARTING_STONES   = 4;

/**
 * @brief The number of pits.
 */
static constexpr inline u64 N_PITS              = 6;

/**
 * @brief The number of stones in the game.
 */
static constexpr inline u64 N_STONES            = N_STARTING_STONES * N_PITS * 2;

/**
 * @brief The number of stones to guarantee a win.
 */
static constexpr inline i32 N_STONES_TO_WIN     = static_cast<i32>((N_STONES / 2) + 1);

/**
 * @brief The evaluation weight/score of having a guaranteed winning position.
 */
constexpr inline i32 EW_WINNING                 = 2'000'000;

/**
 * @brief The evaluation weight of a stone in the mancala.
 */
constexpr inline i32 EW_STONE_IN_MANCALA        = 50;

/**
 * @brief The evaluation weight of a stone in a pit.
 */
constexpr inline i32 EW_STONE_IN_PIT            = 5;
