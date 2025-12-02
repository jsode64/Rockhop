#pragma once

#include "def.h"
#include "side.h"

class MoveList {
protected:
    /** A bitmap of legal moves. */
    u8 moves;

public:
    /** Iterator for moves. */
    class MoveIter {
    private:
        /** A bitmap of legal moves. */
        u8 moves;

    public:
        MoveIter(u8 moves);

        u64 operator*();

        MoveIter& operator++();

        bool operator==(const MoveIter& other);

        bool operator!=(const MoveIter& other);
    };

    MoveList(const Side& side);

    /** Returns `true` if the given move is lega. */
    bool has_move(u64 i);

    /** Returns an iterator over the legal moves. */
    MoveIter begin() const;

    /** The end of a `MoveList::MoveIter`. */
    MoveIter end() const;
};
