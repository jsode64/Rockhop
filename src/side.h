#pragma once

#include <cstdint>

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
     * */
    uint64_t pits;

public:
    Side();

    /**
     * Returns the number of stones in the given pit.
     * Passing 0 will give the mancala (same as `PlayState::mancala`) and 1
     * will give pit #1, 2 pit #2, etc.
     */
    uint64_t pit(uint64_t i) const;

    /** Returns the number of stones in the mancala. */
    uint64_t mancala() const;

    /** Returns `true` if the side has at least one move they can make, `false` if not. */
    bool has_moves() const;

    /** Makes the given move. Returns `true` if the user can move again. */
    bool make_move(uint64_t i, Side& op);

    /** Moves all stones in the pits to the mancala. */
    void take_pits();

private:
    /**
     * Returns the number of stones in the given pit.
     * Assumes the given index is the pit index multiplied by 8.
     */
    uint64_t pit_i(uint64_t i) const;
};
