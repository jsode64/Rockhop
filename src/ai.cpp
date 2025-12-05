#include "ai.h"

#include <algorithm>
#include <functional>
#include <limits>

#include "movelist.h"

static constexpr i32 SCORE_MAX = std::numeric_limits<i32>::max();
static constexpr i32 SCORE_MIN = std::numeric_limits<i32>::min();

std::tuple<u64, i32> AI::find_move(const Game game, const u32 depth) {
    const bool  isPovTurn   = game.is_pov_turn();
    u64         bestMove    = 0;
    i32         bestScore   = isPovTurn ? SCORE_MIN : SCORE_MAX;
    i32         alpha       = SCORE_MIN;
    i32         beta        = SCORE_MAX;

    // Iterate possible moves.
    auto [moves, nMoves] = AI::get_sorted_moves(game);
    for (size i = 0; i < nMoves; i++) {
        const u64 move  = moves[i];
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

std::tuple<std::array<u64, 6>, size> AI::get_sorted_moves(const Game game) {
    using Pair = std::tuple<u64, i32>;
    std::array<Pair, 6> pairs   = {std::tuple(0, 0)};
    std::array<u64, 6>  moves   = {0};
    size                nMoves  = 0;
    const auto          [u, o]  = game.get_turn_user_opp();

    // Get legal moves.
    for (auto move: game.legal_moves()) {
        pairs[nMoves] = std::tuple(move, AI::score_move(u, o, move));
        nMoves++;
    }

    // Sort moves by score.
    std::sort(
        pairs.begin(),
        pairs.begin() + nMoves,
        [](auto a, auto b){ return std::get<1>(a) > std::get<1>(b); }
    );

    // Put move pairs into final array.
    for (size i = 0; i < nMoves; i++) {
        auto [move, _] = pairs[i];
        moves[i] = move;
    }

    return std::tuple(moves, nMoves);
}

__attribute__((hot))
i32 AI::alpha_beta(Game game, const u64 move, const u32 depth, i32 a, i32 b) {
    // This is OK because only legal moves are iterated.
    game.make_move_unchecked(move);

    // Break for depth or game end.
    if (depth < 1 || game.is_over()) {
        return game.eval();
    }

    i32 score = 0;

    // PoV move; find response with highest score.
    if (game.is_pov_turn()) {
        score = SCORE_MIN;

        for (const auto move: game.legal_moves()) {
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

        for (const auto move: game.legal_moves()) {
            score = std::min(score, AI::alpha_beta(game, move, depth - 1, a, b));
            if (score <= a)
                break;
            else
                b = std::min(b, score);
        }
    }

    return score;
}

i32 AI::score_move(const Side u, const Side o, const u64 i) {
    const u64 nStones = u.pit(i);

    if (nStones < i && u.pit(i - nStones) == 0) {
        // Captures are best to try first.
        const u64 opStones = o.pit(7 + nStones - i);
        if (opStones > 0) {
            return 2'000 + opStones;
        }
    } else if (nStones == i) {
        // Chains are good; prioritize ones closer to the mancala.
        return 1'000 - i;
    }

    // Nothing significant about the move.
    return 0;
}
