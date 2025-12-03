#include "game.h"

#include <iostream>
#include <print>

#include "ai.h"
#include "movelist.h"

Game::Game() : man(), bot(), turn(true), isOver(false) {

}

MoveList Game::legal_moves() const {
    if (turn)
        return MoveList(man);
    else
        return MoveList(bot);
}

i32 Game::eval() const {
    return man.eval() - bot.eval();
}

bool Game::is_man_turn() const {
    return turn;
}

bool Game::is_over() const {
    return isOver;
}

void Game::display() const {
    const char turnChar = turn ? 'v' : '^';
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

void Game::make_move(u64 move) {
    if (turn)
        handle_move(man.make_move(move, bot));
    else
        handle_move(bot.make_move(move, man));
}

void Game::move() {
    if (turn)
        man_move();
    else 
        bot_move();
}

void Game::man_move() {
    // Get a valid move from the user.
    MoveList moveList   = MoveList(man);
    u64 move            = -1;
    while (!moveList.has_move(move) && move != 0)
        std::cin >> move;

    if (move == 0) {
        // Giving 0 skips move.
        turn = false;
    } else {
        // Make the move.
        handle_move(man.make_move(move, bot));
    }
}

void Game::bot_move() {
    std::println("Bot thinking...");

    // Get move and evaluation.
    auto [move, eval] = AI::find_move(*this);
    handle_move(bot.make_move(move, man));

    // Print info.
    std::println("Making move: {}", move);
    if (turn)
        std::println("Current evaluation: {}", eval);
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
