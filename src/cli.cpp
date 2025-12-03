#include "cli.h"

#include <charconv>
#include <iostream>
#include <sstream>
#include <print>

#include "ai.h"
#include "def.h"

CLI::CLI() : game(), isOpen(true) {

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
            std::println("{}: Sends the bot into a search then prints the best move and current evaluation.", tok);
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

void CLI::go(std::istringstream&) {
    std::println("Thinking...");
    auto [move, eval] = AI::find_move(game);
    std::println("Best move: {}", move);
    std::println("Evaluation: {}", eval);
}
