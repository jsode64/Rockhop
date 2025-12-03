#pragma once

#include "def.h"
/** A Kalah player's side of the board. */
class Side {
private:
    /** 
     * The Kalah player's pits in a bitmap. 
     * 
     * Each pit it 8 bits as well as the mancala.
     * The 8 least significant bits are the mancala, then the next 8 are pit #1,
     * then pit #2, etc.
     * The last 8 bits are unused.
     */
    u64 pits;

public:
    Side(bool isTurn);

    /**
     * Returns the number of stones in the given pit.
     * Passing 0 will give the mancala (same as `PlayState::mancala`) and 1
     * will give pit #1, 2 pit #2, etc.
     */
    u64 pit(u64 i) const;

    /** Returns the number of stones in the mancala. */
    u64 mancala() const;

    /**
     * Evaluates the side and returns a score for it. A higher score is a more
     * favorable position.
     */
    i32 eval() const;

    /** Returns `true` if the side has at least one move they can make, `false` if not. */
    bool has_moves() const;

    /** Returns `true` if it's this side's turn to move, `false` if not. */
    bool has_turn() const;

    /** Makes the given move. Returns `true` if the user can move again. */
    bool make_move(u64 i, Side& op);

    /** Moves all stones in the pits to the mancala. */
    void take_pits();

    /** Toggles the turn marker for the side. */
    void toggle_turn();

private:
    /**
     * Returns the number of stones in the given pit.
     * Assumes the given index is the pit index multiplied by 8.
     */
    u64 pit_i(u64 i) const;
};
