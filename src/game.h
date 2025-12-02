#pragma once

#include "side.h"

/** A Kalah game state. */
class Game {
private:
    /** The human/user. */
    Side man;

    /** The bot/computer. */
    Side bot;

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

    /** Makes a move. For user turns, reads from the console. For bot turns, calculates the best move. */
    void move();

private:
    /** Reads a move from the user and makes it. */
    void man_move();

    /** Calculates a move for the bot to make and makes it. */
    void bot_move();

    /** Handles the move chaining and game ending. */
    void handle_move(bool chain);
};
