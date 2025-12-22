#pragma once

#include "def.h"

class Side {
private:
    /**
     * @brief the turn bit.
     */
    static constexpr u64 TURN_BIT       = 0x8000000000000000ULL;

    /**
     * @brief A side with all pits holding one stone.
     */
    static constexpr u64 PIT_ONES       = 0x0001010101010100ULL;

    /**
     * @brief A side with all pits and the mancala holding one stone.
     */
    static constexpr u64 ALL_ONES       = 0x0001010101010101ULL;

    /**
     * @brief A bitmask for the mancala.
     */
    static constexpr u64 MAN_MASK       = 0x00000000000000FFULL;

    /**
     * @brief A bitmask 
     */
    static constexpr u64 PIT_MASK       = 0x00FFFFFFFFFFFF00ULL;

    union {
        /**
         * @brief The Kalah player's pits in a bitmap.
         * 
         * @details Each pit it 8 bits as well as the mancala. The 8 least significant
         * bits are the mancala, then the next 8 are pit #1, then pit #2, etc. The last
         * 8 bits are unused.
         */
        u64 pits;

        struct {
            u8 man;
            u8 p1;
            u8 p2;
            u8 p3;
            u8 p4;
            u8 p5;
            u8 p6;
            u8 flags;
        };
    };

public:
    Side(bool isTurn);

    /**
     * @brief Returns the number of stones in the pit at the given index.
     */
    inline i32 pit(u8 i) const {
        return pit_i(i * 8);
    }

    /**
     * @brief Returns the number of stones in the mancala.
     */
    inline i32 mancala() const {
        return static_cast<i32>(man);
    }

    /**
     * @brief Returns `true` if the side has at least one move they can make,
     * `false` if not.
     */
    bool has_moves() const;

    /**
     * @brief Returns `true` if it's this side's turn to move, `false` if not.
     */
    bool has_turn() const;

    /**
     * @brief Makes the given move. Returns `true` if the user can move again.
     */
    bool make_move(u8 move, Side& op);

    /**
     * @brief Moves all stones in the pits to the mancala.
     */
    void take_pits();

    /**
     * @brief Toggles the turn marker for the side.
     */
    void toggle_turn();

private:
    /**
     * @brief Returns the number of stones in the given pit.
     * 
     * @note Assumes the given index is the pit index multiplied by 8.
     */
    inline i32 pit_i(u8 i) const {
        return static_cast<i32>((pits >> i) & Side::MAN_MASK);
    }
};
