#include "ai.h"

#include <algorithm>
#include <functional>
#include <limits>
#include <span>
#include <utility>

#include "movelist.h"

static constexpr i32 SCORE_MAX = std::numeric_limits<i32>::max();

static constexpr i32 SCORE_MIN = std::numeric_limits<i32>::min();

std::tuple<u64, i32> AI::find_move(const Game game, const i32 depth) {
    const bool  isPovTurn   = game.is_pov_turn();
    u64         bestMove    = 0;
    i32         bestScore   = isPovTurn ? SCORE_MIN : SCORE_MAX;
    i32         alpha       = SCORE_MIN;
    i32         beta        = SCORE_MAX;

    // Iterate possible moves.
    auto [moves, nMoves] = AI::get_sorted_moves(game);
    for (size i = 0; i < nMoves; i++) {
        const u64 move  = moves[i].move;
        const i32 score = AI::alpha_beta(game, move, depth - 1, alpha, beta);

        if (isPovTurn) {
            if (score > bestScore) {
                bestMove    = move;
                bestScore   = score;
            }
            alpha = std::max(alpha, bestScore);
        } else {
            if (score < bestScore) {
                bestMove    = move;
                bestScore   = score;
            }
            beta = std::min(beta, bestScore);
        }
    }

    return std::tuple(bestMove, bestScore);
}

__attribute__((hot))
std::tuple<AI::MoveArr, size> AI::get_sorted_moves(const Game game) {
    AI::MoveArr moves   = {ScoredMove()};
    size        nMoves  = 0;
    const auto  [u, o]  = game.get_turn_user_opp();

    // Get legal moves.
    for (auto move: game.legal_moves()) {
        moves[nMoves].move  = move;
        moves[nMoves].score = AI::score_move(u, o, move);
        nMoves++;
    }

    std::span<AI::ScoredMove> validMoves(moves.begin(), nMoves);
    std::sort(
        validMoves.begin(),
        validMoves.end(),
        [](auto a, auto b){ return a.score > b.score; }
    );

    return std::tuple(moves, nMoves);
}

__attribute__((hot))
i32 AI::alpha_beta(Game game, const u64 move, const i32 depth, i32 a, i32 b) {
    // This is OK because only legal moves are iterated.
    game.make_move_unchecked(move);

    // Break for depth or game end.
    if (depth < 1 || game.is_over())
        return game.eval();

    i32     score           = 0;
    auto    [moves, nMoves] = AI::get_sorted_moves(game);

    // PoV move; find response with highest score.
    if (game.is_pov_turn()) {
        score = SCORE_MIN;

        for (size i = 0; i < nMoves; i++) {
            auto move = moves[i].move;
            score = std::max(score, AI::alpha_beta(game, move, depth - 1, a, b));
            if (score >= b)
                break;
            else
                a = std::max(a, score);
        }
    } 

    // Other side move; find response with lowest score.
    else {
        score = SCORE_MAX;

        for (size i = 0; i < nMoves; i++) {
            auto move = moves[i].move;
            score = std::min(score, AI::alpha_beta(game, move, depth - 1, a, b));
            if (score <= a)
                break;
            else
                b = std::min(b, score);
        }
    }

    return score;
}

__attribute__((hot))
i32 AI::score_move(const Side u, const Side o, const u64 i) {
    const u64 nStones = u.pit(i);

    if (nStones < i && u.pit(i - nStones) == 0) {
        // Captures are best to try first.
        const u64 opStones = o.pit(7 + nStones - i);
        if (opStones > 0)
            return 2'000 + opStones;
    } else if (nStones == i)
        // Chains are good; prioritize ones closer to the mancala.
        return 1'000 - i;

    // Nothing significant about the move.
    return 0;
}
