#include "side.h"

#include <algorithm>

#include "config.h"
#include "def.h"

Side::Side(const bool isTurn) : pits(Side::PIT_ONES * N_STARTING_STONES) {
    // Activate turn bit if it's this side's turn.
    if (isTurn)
        pits |= Side::TURN_BIT;
}

bool Side::has_moves() const {
    return (pits & Side::PIT_MASK) != 0;
}

bool Side::has_turn() const {
    return (pits & Side::TURN_BIT) != 0;
}

__attribute__((hot))
bool Side::make_move(const u64 i, Side& op) {
    // Take the stones out of the pit.
    const u64 p = i * 8;
    u64 nStones = static_cast<u64>(pit_i(p));
    pits -= nStones << p;

    // Place pits across the board.
    const u64 nCycles = nStones / 13;
    pits    += ALL_ONES * nCycles;
    op.pits += PIT_ONES * nCycles;
    nStones %= 13;

    // Place leftover pits on user's side.
    u64 myMask              = ~0ULL >> (64 - p);                            // Mask to start from chosen pit.
    if (nStones < i)        myMask &= ~0ULL << (8 * (i - nStones));         // Mask to last pit.
    if (nStones > i + 6)    myMask |= ~(~0ULL >> (8 * (nStones - i - 5)));  // Mask to wrapping back around.
    pits += Side::ALL_ONES & myMask;

    // Place overflown pits on opponent's side.
    u64 opMask = 0;
    if (nStones > i) opMask |= 0x00FFFFFFFFFFFF00ULL << (8 * (6 - std::min(u64{6}, nStones - i)));
    op.pits += Side::PIT_ONES & opMask;

    // Get the last position and tell if it was a capture or mancala chain.
    u64 last                = (6 - i) + nStones;
    const bool lastIsUser   = last < 6;
    const bool lastIsMan    = last == 6;

    if (lastIsUser) {
        const u64 myPitI       = 8 * (6 - last);
        const u64 opPitI       = 8 * (last + 1);
        const u64 nMyStones    = pit_i(myPitI);
        const u64 nOpStones    = op.pit_i(opPitI);

        if (nMyStones == 1 && nOpStones > 0) {
            // Claim stones and set pits to zero.
            pits    += nMyStones + nOpStones;
            pits    -= nMyStones << myPitI;
            op.pits -= nOpStones << opPitI;
        }
    } else if (lastIsMan) {
        // Chain moves.
        return true;
    }

    return false;
}

void Side::take_pits() {
    // Add all pits to the mancala.
    pits += p1;
    pits += p2;
    pits += p3;
    pits += p4;
    pits += p5;
    pits += p6;
    
    // Set all pits to be empty.
    pits &= ~Side::PIT_MASK;
}

void Side::toggle_turn() {
    pits ^= Side::TURN_BIT;
}
