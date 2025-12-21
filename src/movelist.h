#pragma once

#include "def.h"
#include "side.h"

class MoveList {
protected:
    /**
     * @brief A bitmap of legal moves.
     */
    u8 moves;

public:
    /**
     * @brief Iterator for moves.
     */
    class MoveIter {
    private:
        /**
         * @brief A bitmap of legal moves.
         */
        u8 moves;

    public:
        MoveIter(u8 moves);

        u64 operator*();

        MoveIter& operator++();

        bool operator==(const MoveIter& other);

        bool operator!=(const MoveIter& other);
    };

    MoveList(const Side& side);

    /**
     * @brief Returns `true` if the given move is legal.
     */
    bool has_move(u64 i);

    /**
     * @brief Returns an iterator over the legal moves.
     */
    MoveIter begin() const;

    /**
     * @brief The end of a `MoveList::MoveIter`.
     */
    MoveIter end() const;
};
