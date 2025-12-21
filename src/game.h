#pragma once

#include <tuple>

#include "def.h"
#include "movelist.h"
#include "side.h"

class Game {
private:
    /**
     * @brief The lower/PoV side.
     */
    Side a;

    /**
     * @brief The upper side.
     */
    Side b;

public:
    Game();

    /**
     * @brief Returns an iterator of the current legal moves.
     */
    MoveList legal_moves() const;

    /**
     * @brief Returns the PoV side and the opponent side in a tuple.
     */
    std::tuple<Side, Side> get_sides() const;

    /**
     * @brief Returns whose turn it is and their opponent in a tuple.
     */
    std::tuple<Side, Side> get_turn_user_opp() const;

    /**
     * @brief Returns an evaluation of the current position.
     * 
     * The evaluation comes from the human's PoV, meaning a larger score is
     * beneficial to the human, a lower score is beneficial for the bot.
     */
    i32 eval() const;

    /**
     * @brief Returns `true` if it's the PoV side's turn, `false` if not.
     */
    bool is_pov_turn() const;

    /**
     * @brief Returns `true` if the game is over, `false` if not.
     */
    bool is_over() const;

    /**
     * @brief Prints the game state.
     */
    void display() const;

    /**
     * @brief Makes the given move.
     * 
     * If the move is not legal, returns `false` without changing the game state.
     * If the move is legal, plays it and returns `true`.
    */
    bool make_move(u64 move);

    /**
     * @brief Makes the given move without checking if it is legal.
     * 
     * If called with an illegal move, the turns will just be swapped.
     */
    void make_move_unchecked(u64 move);

private:
    /**
     * @brief Returns references of whose turn it is to move next and their opponent.
     */
    std::tuple<Side&, Side&> get_turn_user_opp_ref();

    /**
     * @brief Handles the move chaining and game ending.
     */
    void handle_move(bool chain);

    /**
     * @brief Swaps whose turn it is to move.
     */
    void swap_turn();
};
