#include "ai.h"

#include <algorithm>
#include <functional>
#include <limits>
#include <span>
#include <utility>

#include "movelist.h"

static constexpr i32 SCORE_MAX = std::numeric_limits<i32>::max();

static constexpr i32 SCORE_MIN = std::numeric_limits<i32>::min();

AI::ScoredMove::ScoredMove(size i, i32 score) : i(i), score(score) {

}

std::tuple<u64, i32> AI::find_move(const Game game, const i32 depth) {
    const bool  isPovTurn   = game.is_pov_turn();
    u64         bestMove    = 0;
    i32         bestScore   = isPovTurn ? SCORE_MIN : SCORE_MAX;
    i32         alpha       = SCORE_MIN;
    i32         beta        = SCORE_MAX;

    // Iterate possible moves.
    for (const auto move: get_sorted_moves(game)) {
        const i32 score = alpha_beta(game, move, depth - 1, alpha, beta);

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
MoveList AI::get_sorted_moves(const Game game) {
    MoveList                        legalMoves  = game.legal_moves();
    const size                      nMoves      = legalMoves.n_moves();
    std::array<ScoredMove, N_PITS>  scoredMoves = {};
    const auto                      [u, o]      = game.get_turn_user_opp();

    // Score legal moves.
    for (size i = 0; i < nMoves; i++)
        scoredMoves[i] = ScoredMove(i, score_move(u, o, legalMoves[i]));

    // Sort moves based on score.
    std::span<ScoredMove> movesToSort(scoredMoves.begin(), nMoves);
    std::sort(
        movesToSort.begin(),
        movesToSort.end(),
        [](auto a, auto b){ return a.score > b.score; }
    );
    
    // Get final sorted move list.
    MoveList orderedMoves(nMoves);
    for (size i = 0; i < nMoves; i++)
        orderedMoves[i] = legalMoves[scoredMoves[i].i];

    return orderedMoves;
}

__attribute__((hot))
i32 AI::alpha_beta(Game game, const u8 move, const i32 depth, i32 a, i32 b) {
    // This is OK because only legal moves are iterated.
    game.make_move_unchecked(move);

    // Break for depth or game end.
    if (depth < 1 || game.is_over())
        return game.eval();

    i32     score   = 0;
    auto    moves   = get_sorted_moves(game);

    // PoV move; find response with highest score.
    if (game.is_pov_turn()) {
        score = SCORE_MIN;

        for (const auto move: moves) {
            score = std::max(score, alpha_beta(game, move, depth - 1, a, b));
            if (score >= b)
                break;
            else
                a = std::max(a, score);
        }
    } 

    // Other side move; find response with lowest score.
    else {
        score = SCORE_MAX;

        for (const auto move: moves) {
            score = std::min(score, alpha_beta(game, move, depth - 1, a, b));
            if (score <= a)
                break;
            else
                b = std::min(b, score);
        }
    }

    return score;
}

__attribute__((hot))
i32 AI::score_move(const Side u, const Side o, const u8 i) {
    const u8 nStones = static_cast<u8>(u.pit(i));

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
