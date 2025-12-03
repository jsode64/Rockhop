#include "side.h"

#include <algorithm>

#include "def.h"

/** A bitmap of all pits holding 1 stone. */
static constexpr u64 PIT_ONES       = 0x0001010101010100ULL;

/** A bitmap of all pits and the mancala holding 1 stone. */
static constexpr u64 ALL_ONES       = 0x0001010101010101ULL;

/** A bitmask for the mancala. */
static constexpr u64 MAN_MASK       = 0x00000000000000FFULL;

/** A mask to all pit bits. */
static constexpr u64 PIT_MASK       = 0x00FFFFFFFFFFFF00ULL;

/** The number of pits on each side. */
static constexpr u64 N_PITS         = 6;

/** The number of stones in each pit at the game start. */
static constexpr u64 N_START_STONES = 4;

/** The score returned for a winning position. */
static constexpr i32 WIN_SCORE      = 5'000'000;

Side::Side() : pits(PIT_ONES * N_START_STONES) {

}

u64 Side::pit(const u64 i) const {
    return (pits >> (i * 8)) & MAN_MASK;
}

u64 Side::mancala() const {
    return pits & MAN_MASK;
}

i32 Side::eval() const {
    u64 nMancala    = mancala();
    i32 score       = 0;

    // Guaranteed win.
    if (nMancala > N_START_STONES * N_PITS)
        return WIN_SCORE;

    // Favore a bountiful mancala.
    score += nMancala * 10;

    // Analyze pits:
    for (u64 i = 1; i < N_PITS; i++) {
        u64 nStones = pit(i);

        if (nStones == 0) {
            // Favor empty pits for captures.
            score += 3;
        } else if (nStones < i) {
            // Favor underflown pits.
            score += nStones + 2;
        } else {
            // Avoid overflown pits.
            score -= std::min(6ULL, nStones - i);
        }
    }

    return score;
}

bool Side::has_moves() const {
    return (pits & PIT_MASK) != 0;
}

bool Side::make_move(const u64 i, Side& op) {
    const u64 p = i * 8;

    // Take the stones out of the pit.
    u64 nStones = pit_i(p);
    pits -= nStones << p;

    // Place pits across the board.
    const u64 nCycles = nStones / 13;
    pits    += ALL_ONES * nCycles;
    op.pits += PIT_ONES * nCycles;
    nStones %= 13;

    // Place leftover pits on user's side.
    u64 myMask                  = ~0ULL >> (64 - p);                    // Mask to start from chosen pit.
    if (nStones < i) myMask     &= ~0ULL << (8 * (i - nStones));        // Mask to last pit.
    if (nStones > i + 6) myMask |= ~(~0ULL >> (8 * (nStones - i - 5))); // Mask to wrapping back around.
    pits += ALL_ONES & myMask;

    // Place overflown pits on opponent's side.
    u64 opMask              = 0;
    if (nStones > i) opMask |= 0x00FFFFFFFFFFFF00ULL << (8 * (6 - std::min(6ULL, nStones - i)));
    op.pits += PIT_ONES & opMask;

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
    pits += (pits >> 8)     & MAN_MASK;
    pits += (pits >> 16)    & MAN_MASK;
    pits += (pits >> 24)    & MAN_MASK;
    pits += (pits >> 32)    & MAN_MASK;
    pits += (pits >> 40)    & MAN_MASK;
    pits += (pits >> 48)    & MAN_MASK;
    
    // Set all pits to be empty.
    pits &= ~PIT_MASK;
}

u64 Side::pit_i(u64 i) const {
    return (pits >> i) & MAN_MASK;
}
