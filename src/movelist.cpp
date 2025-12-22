#include "movelist.h"

#include <bit>

#include "config.h"

MoveList::MoveList(const Side& side) : moves(0) {
    // Mark moves for non-empty pits.
    for (u64 i = 1; i <= N_PITS; i++) {
        if (side.pit(i) > 0)
            moves |= 1ULL << i;
    }
}

bool MoveList::has_move(u64 i) {
    return (moves & (1ULL << i)) != 0;
}

MoveList::MoveIter MoveList::begin() const {
    return MoveIter(moves);
}

MoveList::MoveIter MoveList::end() const {
    return MoveIter(0);
}

MoveList::MoveIter::MoveIter(const u8 moves) : moves(moves) {

}

u64 MoveList::MoveIter::operator*() {
    return std::countr_zero(moves);
}

MoveList::MoveIter& MoveList::MoveIter::operator++() {
    moves ^= 1ULL << std::countr_zero(moves);
    return *this;
}

bool MoveList::MoveIter::operator==(const MoveIter& other) {
    return moves == other.moves;
}

bool MoveList::MoveIter::operator!=(const MoveIter& other) {
    return moves != other.moves;
}
