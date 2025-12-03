#pragma once

#include <tuple>

#include "def.h"
#include "movelist.h"
#include "side.h"

/** A Kalah game state. */
class Game {
private:
    /** The lower/PoV side. */
    Side a;

    /** The upper side. */
    Side b;

public:
    Game();

    /** Returns an iterator of the current legal moves. */
    MoveList legal_moves() const;

    /** 
     * Returns an evaluation of the current position. 
     * 
     * The evaluation comes from the human's PoV, meaning a larger score is
     * beneficial to the human, a lower score is beneficial for the bot.
     */
    i32 eval() const;

    /** Returns `true` if it's the PoV side's turn, `false` if not. */
    bool is_pov_turn() const;

    /** Returns `true` if the game is over, `false` if not. */
    bool is_over() const;

    /** Prints the game state. */
    void display() const;

    /**
     * Makes the given move.
     * 
     * If the move is not legal, returns `false` without changing the game state.
     * If the move is legal, plays it and returns `true`.
    */
    bool make_move(u64 move);

    /**
     * Makes the given move without checking if it is legal.
     * 
     * If called with an illegal move, the turns will just be swapped.
     */
    void make_move_unchecked(u64 move);

private:
    /** Returns whose turn it is to move next and their opponent. */
    std::tuple<Side&, Side&> get_turn_user_opp();

    /** Handles the move chaining and game ending. */
    void handle_move(bool chain);

    /** Swaps whose turn it is to move. */
    void swap_turn();
};
