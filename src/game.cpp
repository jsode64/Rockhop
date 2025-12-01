#include "game.h"

#include <iostream>
#include <print>
#include <string>

Game::Game() : a(), b(), turn(true), isOver(false) {

}

bool Game::is_over() const {
    return isOver;
}

void Game::display() const {
    char turnChar = turn ? 'v' : '^';
    std::print(
        "------------------------\n"
        "   1  2  3  4  5  6\n"
        "   {:02} {:02} {:02} {:02} {:02} {:02}\n"
        "{:02}                   {:02}  {}TURN{}\n"
        "   {:02} {:02} {:02} {:02} {:02} {:02}\n"
        "    6  5  4  3  2  1\n"
        "------------------------\n",
        b.pit(1), b.pit(2), b.pit(3), b.pit(4), b.pit(5), b.pit(6),
        b.mancala(), a.mancala(), turnChar, turnChar,
        a.pit(6), a.pit(5), a.pit(4), a.pit(3), a.pit(2), a.pit(1)
    );
}

void Game::user_move() {
    // Wait for a valid move from the user.
    uint64_t i;
    std::cin >> i;

    make_move(i);
}

void Game::make_move(uint64_t i) {
    // Make the move for whose turn it is.
    bool isChain    = false;
    if (turn) {
        isChain = a.make_move(i, b);
    } else {
        isChain = b.make_move(i, a);
    }

    // Swap turns if not chaining.
    if (!isChain)
        turn = !turn;

    // Check for game ending.
    isOver =
        (turn && !a.has_moves())
        || (!turn && !b.has_moves());
    if (isOver) {
        a.take_pits();
        b.take_pits();
    }
}