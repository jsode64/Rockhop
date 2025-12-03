#pragma once

#include "def.h"
#include "movelist.h"
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

    /** Returns an iterator of the current legal moves. */
    MoveList legal_moves() const;

    /** 
     * Returns an evaluation of the current position. 
     * 
     * The evaluation comes from the human's PoV, meaning a larger score is
     * beneficial to the human, a lower score is beneficial for the bot.
     */
    i32 eval() const;

    /** Returns `true` if it's the human's turn, `false` if not. */
    bool is_man_turn() const;

    /** Returns `true` if the game is over, `false` if not. */
    bool is_over() const;

    /** Prints the game state. */
    void display() const;

    /** Makes the given move. */
    void make_move(u64 move);

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
