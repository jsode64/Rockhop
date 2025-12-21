#pragma once

#include <array>
#include <tuple>

#include "def.h"
#include "game.h"
#include "side.h"

class AI {
private:
    /**
     * @brief A move and its score used for move ordering.
     */
    struct ScoredMove {
        /**
         * @brief The move.
         */
        u64 move;

        /**
         * @brief The move's score.
         */
        i32 score;

        constexpr ScoredMove() : move(0), score(0) {

        }
    };

    /**
     * @brief An array of moves.
     */
    using MoveArr = std::array<AI::ScoredMove, 6>;

public:
    /**
     * @brief Searches to the given depth and returns the optimal move found and the evaluation.
     */
    static std::tuple<u64, i32> find_move(Game game, i32 depth);

private:
    /** 
     * @brief Returns the legal moves sorted by instant potential in ascending order.
     */
    static std::tuple<AI::MoveArr, size> get_sorted_moves(Game game);

    /** 
     * @brief Alpha beta prune depth search.
     */
    static i32 alpha_beta(Game game, u64 move, i32 depth, i32 a, i32 b);

    /**
     * @brief Scores the given move.
     */
    static i32 score_move(Side u, Side o, u64 i);
};
