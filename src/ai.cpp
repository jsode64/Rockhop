#include "ai.h"

#include <algorithm>
#include <functional>
#include <limits>

#include "movelist.h"

static constexpr i32 SCORE_MAX = std::numeric_limits<i32>::max();
static constexpr i32 SCORE_MIN = std::numeric_limits<i32>::min();

std::tuple<u64, i32> AI::find_move(Game game) {
    u64 bestMove    = 0;
    i32 bestScore   = SCORE_MAX;

    // Iterate possible moves.
    MoveList moves = game.legal_moves();
    for (auto move: moves) {
        i32 score = AI::score_move(game, move, 20, SCORE_MIN, SCORE_MAX);

        // Use move if it scores better for the bot (lower score is better).
        if (score < bestScore) {
            bestMove    = move;
            bestScore   = score;
        }
    }

    return std::tuple(bestMove, bestScore);
}

i32 AI::score_move(Game game, u64 move, i32 depth, i32 a, i32 b) {
    game.make_move(move);

    // Break for depth or game end.
    if (depth < 1 || game.is_over()) {
        return game.eval();
    }

    i32 score = 0;

    // Player move; find response with highest score.
    if (game.is_man_turn()) {
        score = SCORE_MIN;

        for (const auto move: game.legal_moves()) {
            score = std::max(score, AI::score_move(game, move, depth - 1, a, b));
            if (score >= b)
                break;
            else
                a = std::max(a, score);
        }
    } 

    // Bot move; find response with lowest score.
    else {
        score = SCORE_MAX;

        for (const auto move: game.legal_moves()) {
            score = std::min(score, AI::score_move(game, move, depth - 1, a, b));
            if (score <= a)
                break;
            else
                b = std::min(b, score);
        }
    }

    return score;
}
