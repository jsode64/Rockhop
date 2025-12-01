#pragma once

#include "side.h"

/** A Kalah game state. */
class Game {
private:
    /** The user. */
    Side a;

    /** The computer. */
    Side b;

    /** Turn tracker. Is `true` on user's turn, `false` on computer's. */
    bool turn;

    /** Is `true` when the game is over, `false` if still going. */
    bool isOver;

public:
    Game();

    /** Returns `true` if the game is over, `false` if not. */
    bool is_over() const;

    /** Prints the game state. */
    void display() const;

    /** Reads a move from the user and makes it. */
    void user_move();

private:
    /** Makes the given move. */
    void make_move(uint64_t i);
};
