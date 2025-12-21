#pragma once

#include "def.h"

class Side {
private:
    /**
     * @brief The Kalah player's pits in a bitmap.
     * 
     * @details Each pit it 8 bits as well as the mancala. The 8 least significant
     * bits are the mancala, then the next 8 are pit #1, then pit #2, etc. The last
     * 8 bits are unused.
     */
    u64 pits;

public:
    Side(bool isTurn);

    /**
     * @brief Returns the number of stones in the given pit.
     * 
     * @details Passing 0 will give the mancala (same as `PlayState::mancala`) 
     * and 1 will give pit #1, 2 pit #2, etc.
     */
    u64 pit(u64 i) const;

    /**
     * @brief Returns the number of stones in the mancala.
     */
    u64 mancala() const;

    /**
     * @brief Grades the given pit and returns its score.
     */
    i32 grade_pit(u64 i, Side op) const;

    /**
     * @brief Returns `true` if the side has at least one move they can make,
     * `false` if not.
     */
    bool has_moves() const;

    /**
     * @brief Returns `true` if it's this side's turn to move, `false` if not.
     */
    bool has_turn() const;

    /**
     * @brief Makes the given move. Returns `true` if the user can move again.
     */
    bool make_move(u64 i, Side& op);

    /**
     * @brief Moves all stones in the pits to the mancala.
     */
    void take_pits();

    /**
     * @brief Toggles the turn marker for the side.
     */
    void toggle_turn();

private:
    /**
     * @brief Returns the number of stones in the given pit.
     * 
     * @note Assumes the given index is the pit index multiplied by 8.
     */
    u64 pit_i(u64 i) const;
};
