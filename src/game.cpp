#include "game.h"

#include <iostream>
#include <print>
#include <random>
#include <string>

#include "movelist.h"

Game::Game() : man(), bot(), turn(true), isOver(false) {

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
        bot.pit(1), bot.pit(2), bot.pit(3), bot.pit(4), bot.pit(5), bot.pit(6),
        bot.mancala(), man.mancala(), turnChar, turnChar,
        man.pit(6), man.pit(5), man.pit(4), man.pit(3), man.pit(2), man.pit(1)
    );
}

void Game::move() {
    if (turn)
        man_move();
    else 
        bot_move();
}

void Game::man_move() {
    // Get a valid move from the user.
    MoveList moveList = MoveList(man);
    u64 i = 0;
    while (!moveList.has_move(i)) {
        std::cin >> i;
    }

    // Make the move.
    handle_move(man.make_move(i, bot));
}

void Game::bot_move() {
    // Choose a random move.
    MoveList moveList = MoveList(bot);
    u64 i = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    size n = 0;
    for (const auto move: moveList) {
        // Reservoir sample.
        std::uniform_int_distribution<u64> dist(0, n);
        if (dist(gen) == 0) {
            i = move;
        }

        n++;
    }

    // Make the move.
    handle_move(bot.make_move(i, man));
}

void Game::handle_move(bool chain) {
    // Change turns if not chaining.
    if (!chain)
        turn = !turn;

    // Check for game ending.
    isOver =
        (turn && !man.has_moves())
        || (!turn && !bot.has_moves());

    // Handle game ending.
    if (isOver) {
        man.take_pits();
        bot.take_pits();
    }
}
