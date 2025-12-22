#pragma once

#include <array>

#include "config.h"
#include "def.h"
#include "side.h"

class MoveList {
private:
    /**
     * @brief An array of legal moves.
     */
    std::array<u8, N_PITS>  moves;

    /**
     * @brief The number of moves.
     */
    size nMoves;

public:
    /**
     * @brief A move list from the given side.
     */
    explicit MoveList(Side side);

    /**
     * @brief A move list with the given number of moves.
     * 
     * @warning The moves must be defined explicitly and are uninitialized otherwise.
     */
    explicit MoveList(size nMoves);

    inline auto begin() {
        return moves.begin();
    }

    inline auto end() {
        return moves.begin() + nMoves;
    }

    inline u8& operator[](size i) {
        return moves[i];
    }

    inline size n_moves() const {
        return nMoves;
    }

    /**
     * @brief Returns `true` if the given move is in the move list, `false` if not.
     */
    bool has_move(u8 move) const;
};
