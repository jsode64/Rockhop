#include "game.h"

#include <print>

#include "movelist.h"

Game::Game() : a(true), b(false) {

}

MoveList Game::legal_moves() const {
    if (a.has_turn())
        return MoveList(a);
    else
        return MoveList(b);
}

i32 Game::eval() const {
    return a.eval() - b.eval();
}

bool Game::is_pov_turn() const {
    return a.has_turn();
}

bool Game::is_over() const {
    return !(a.has_moves() && b.has_moves());
}

void Game::display() const {
    const char turnChar = a.has_turn() ? 'v' : '^';
    std::print(
        "------------------------\n"
        "   1  2  3  4  5  6\n"
        "   {:02} {:02} {:02} {:02} {:02} {:02}\n"
        "{:02}                   {:02}  {}TURN{}\n"
        "   {:02} {:02} {:02} {:02} {:02} {:02}\n"
        "    6  5  4  3  2  1\n"
        "------------------------\n",
        b.pit(1), b.pit(2), b.pit(3), b.pit(4), b.pit(5), b.pit(6),
        b.mancala(), a.mancala(),
        turnChar, turnChar,
        a.pit(6), a.pit(5), a.pit(4), a.pit(3), a.pit(2), a.pit(1)
    );
}

bool Game::make_move(u64 move) {
    auto [u, o] = get_turn_user_opp();

    if (MoveList(u).has_move(move)) {
        // Make the move.
        handle_move(u.make_move(move, o));
        return true;
    } else {
        // Move was illegal.
        return false;
    }
}

void Game::make_move_unchecked(u64 move) {
    auto [u, o] = get_turn_user_opp();
    handle_move(u.make_move(move, o));
}

std::tuple<Side&, Side&> Game::get_turn_user_opp() {
    return a.has_turn()
        ? std::tie(a, b)
        : std::tie(b, a);
}

void Game::handle_move(bool chain) {
    // Change turns if not chaining.
    if (!chain)
        swap_turn();

    // Handle game ending.
    if (is_over()) {
        a.take_pits();
        b.take_pits();
    }
}

void Game::swap_turn() {
    a.toggle_turn();
    b.toggle_turn();
}
