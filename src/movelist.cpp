#include "movelist.h"


MoveList::MoveList(Side side) : moves{}, nMoves(0) {
    // Any non-empty pit is a legal move.
    for (u8 i = 1; i <= N_PITS; i++) {
        if (side.pit(i) > 0) {
            moves[nMoves] = i;
            nMoves++;
        }
    }
}

MoveList::MoveList(size nMoves) : moves{}, nMoves(nMoves) {

}

bool MoveList::has_move(u8 move) const {
    for (size i = 0; i < nMoves; i++)
        if (moves[i] == move)
            return true;

    return false;
}
