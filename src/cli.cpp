#include "cli.h"

#include <charconv>
#include <iostream>
#include <optional>
#include <print>
#include <sstream>

#include "ai.h"
#include "def.h"

/** Parses the given string to an unsigned integer and returns it, or `nullopt` if invalid. */
std::optional<u32> parse_uint(const std::string& s);

CLI::CLI() : game(), isOpen(true) {
    std::println("Rockhop v1.0");
}

bool CLI::is_open() const {
    return isOpen;
}

void CLI::process() {
    // Get line from user.
    std::string input;
    std::getline(std::cin, input);

    // Match the first token to a command.
    std::istringstream toks(input);
    std::string cmd;
    toks >> std::skipws >> cmd;
    if (cmd == "q" || cmd == "quit")
        quit(toks);
    else if (cmd == "h" || cmd == "help")
        help(toks);
    else if (cmd == "m" || cmd == "move")
        move(toks);
    else if (cmd == "d" || cmd == "display")
        display(toks);
    else if (cmd == "p" || cmd == "pos" || cmd == "position")
        position(toks);
    else if (cmd == "go")
        go(toks);
    else if (cmd == "e" || cmd == "eval")
        eval(toks);
    else
        std::println("Unknown comand: \"{}\"", cmd);
    
    std::println("");
}

void CLI::quit(std::istringstream&) {
    isOpen = false;
}

void CLI::help(std::istringstream& toks) {
    std::string tok;
    while (toks >> tok) {
        if (tok == "q" || tok == "quit")
            std::println("{}: Ends the program.", tok);
        else if (tok == "h" || tok == "help")
            std::println("{}: Displays helpful information for the given commands.", tok);
        else if (tok == "m" || tok == "move")
            std::println(
                "{}: Makes the given moves in order. Example: \"m 4 1 5 6\"."
                "\n  If any input is an illegal or invalid move, the game state will not change.",
                tok
            );
        else if (tok == "d" || tok == "display")
            std::println("{}: Displays the current game state.", tok);
        else if (tok == "p" || tok == "pos" || tok == "position")
            std::println("{}: Sets the position. Example: \"p startpos\".", tok);
        else if (tok == "go")
            std::println(
                "{}: Find the best move and make it the given number of times. Example: \"go depth 22 play 4\""
                "\n  If depth is not specified, defaults to 14."
                "\n  If number of moves to play is not specified, defaults to 1.",
                tok
            );
        else if (tok == "e" || tok == "eval")
            std::println(
                "{}: Get the best move and current evaluation with the given depth. Example: \"eval depth 12\"."
                "\n  If depth is not specified, defaults to 14.",
                tok
            );
        else
            std::println("Unknown command \"{}\", ignoring.", tok);
    }
}

void CLI::move(std::istringstream& toks) {
    std::string tok;
    Game result = game;
    size i = 1;

    while (toks >> tok) {
        // Convert move to integer.
        u64 move = 0;
        auto [_, r] = std::from_chars(tok.data(), tok.data() + tok.size(), move);

        if (r != std::errc{}) {
            // Invalid token.
            std::println("Token #{} \"{}\" is not a move. Game state unchanged.", i, tok);
            return;
        } else if (move > 6 || move < 1) {
            // Invalid move index.
            std::println("Token #{} \"{}\" is not within the range [1,6]. Game state unchanged.", i, move);
            return;
        } else {
            if (!result.make_move(move)) {
                // Move was illegal.
                std::println("Move #{} ({}) was not legal. Game state unchanged.", i, move);
                return;
            }
        }

        i++;
    }

    // If here, all moves worked and the game state can be updated.
    game = result;
}

void CLI::display(std::istringstream&) {
    game.display();
}

void CLI::position(std::istringstream& toks) {
    std::string tok;
    toks >> tok;
    if (tok == "startpos")
        game = Game();
    else
        std::println("Unknown position argument: \"{}\". Game state unchanged.", tok);
}

void CLI::go(std::istringstream& toks) {
    // Get the depth options.
    u32 depth   = 14;
    u32 nMoves  = 1;
    std::string tok;
    while (toks >> tok) {
        if (tok == "depth") {
            // Get and set depth.
            toks >> tok;
            auto n = parse_uint(tok);
            if (n)
                depth = n.value();
            else {
                std::println("Expected unsigned integer for depth, found \"{}\".", tok);
                return;
            }
        } else if (tok == "play") {
            // Get and set number of moves to play.
            toks >> tok;
            auto n = parse_uint(tok);
            if (n)
                nMoves = n.value();
            else {
                std::println("Expected unsigned integer for number of moves to play, found \"{}\".", tok);
                return;
            }
        }
    }

    for (u32 i = 0; i < nMoves; i++) {
        // Stop early if game ends.
        if (game.is_over()) {
            std::println("Game ended. Ending search.");
            break;
        }

        // Get the best move.
        std::println("Thinking with depth {}...", depth);
        auto [move, _] = AI::find_move(game, depth);

        // Say and make it.
        std::println("Playing move {}.", move);
        game.make_move(move);
    }
}

void CLI::eval(std::istringstream& toks) {
    u32 depth = 14;

    // See if a depth was given.
    std::string tok;
    if (toks >> tok) {
        if (tok == "depth") {
            // Get and set depth.
            toks >> tok;
            auto n = parse_uint(tok);
            if (n)
                depth = n.value();
            else {
                std::println("Expected unsigned integer for depth, found \"{}\"", tok);
                return;
            }
        } else {
            std::println("Unknown argument \"{}\"", tok);
            return;
        }
    }

    // Get evaluation.
    std::println("Evaluating with depth {}...", depth);
    auto [move, eval] = AI::find_move(game, depth);
    std::println("Best move:   {}", move);
    std::println("Evaluation:  {}", eval);
}

std::optional<u32> parse_uint(const std::string& s) {
    // Attempt to parse to integer.
    u32 n = 0;
    auto [_, e] = std::from_chars(s.data(), s.data() + s.size(), n);

    if (e == std::errc{})
        return n;
    else
        // Invalid integer.
        return std::nullopt;
}
