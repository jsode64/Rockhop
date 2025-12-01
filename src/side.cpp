#include "side.h"

#include <algorithm>
#include <print>

/** A bitmap of all pits holding 1 stone. */
static constexpr uint64_t PIT_ONES = 0x0001010101010100ULL;

/** A bitmap of all pits and the mancala holding 1 stone. */
static constexpr uint64_t ALL_ONES = 0x0001010101010101ULL;

/** A bitmask for the mancala. */
static constexpr uint64_t MAN_MASK = 0x00000000000000FFULL;

/** A mask to all pit bits. */
static constexpr uint64_t PIT_MASK = 0x00FFFFFFFFFFFF00ULL;

Side::Side() : pits(PIT_ONES * 4) {

}

uint64_t Side::pit(const uint64_t i) const {
    return (pits >> (i * 8)) & MAN_MASK;
}

uint64_t Side::mancala() const {
    return pits & MAN_MASK;
}

bool Side::has_moves() const {
    return (pits & PIT_MASK) != 0;
}

bool Side::make_move(const uint64_t i, Side& op) {
    const uint64_t p = i * 8;

    // Take the stones out of the pit.
    uint64_t nStones = pit_i(p);
    pits -= nStones << p;

    // Place pits across the board.
    const uint64_t nCycles = nStones / 13;
    pits    += ALL_ONES * nCycles;
    op.pits += PIT_ONES * nCycles;
    nStones %= 13;

    // Place leftover pits on user's side.
    uint64_t myMask             = ~0ULL >> (64 - p);                    // Mask to start from chosen pit.
    if (nStones < i) myMask     &= ~0ULL << (8 * (i - nStones));        // Mask to last pit.
    if (nStones > i + 6) myMask |= ~(~0ULL >> (8 * (nStones - i - 5))); // Mask to wrapping back around.
    pits += ALL_ONES & myMask;

    // Place overflown pits on opponent's side.
    const uint64_t opMask = ~(0x00FFFFFFFFFFFFFFULL >> (8 * (nStones - std::min(nStones, i))));
    op.pits += PIT_ONES & opMask;

    // Get the last position and tell if it was a capture or mancala chain.
    uint64_t last           = (6 - i) + nStones;
    const bool lastIsUser   = last < 6;
    const bool lastIsMan    = last == 6;

    if (lastIsUser) {
        const uint64_t myPitI       = 8 * (6 - last);
        const uint64_t opPitI       = 8 * (last + 1);
        const uint64_t nMyStones    = pit_i(myPitI);
        const uint64_t nOpStones    = op.pit_i(opPitI);

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

uint64_t Side::pit_i(uint64_t i) const {
    return (pits >> i) & MAN_MASK;
}
