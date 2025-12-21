#pragma once

#include <sstream>

#include "game.h"

class CLI {
private:
    /**
     * @brief The default depth for "go" and "eval".
     */
    static constexpr inline i32 DEFAULT_DEPTH = 21;

    /**
     * @brief Game state.
     */
    Game game;

    /**
     * @brief Is `true` when the CLI was closed, `false` if not.
     */
    bool isOpen;

public:
    CLI();

    /**
     * @brief Returns `true` if the CLI has not been stopped, `false` if it has.
     */
    bool is_open() const;

    /**
     * @brief Waits for, reads, and processes the user's next command.
     */
    void process();

private:
    /**
     * @brief Handles "q" or "quit".
     * 
     * Closes the CLI.
     */
    void quit(std::istringstream& toks);

    /**
     * @brief Handles "h" or "help".
     * 
     * Displays help for the given commands.
     */
    void help(std::istringstream& toks);

    /**
     * @brief Handles "m" or "move".
     * 
     * Reads the move(s) and makes them.
     */
    void move(std::istringstream& toks);

    /**
     * @brief Handles "d" or "display".
     * 
     * Prints the game state.
     */
    void display(std::istringstream& toks);

    /**
     * @brief Handles "p", "pos", or "position".
     * 
     * Sets the position.
     */
    void position(std::istringstream& toks);

    /**
     * @brief Handles "go".
     * 
     * Plays a specified number of moves (default 1).
     */
    void go(std::istringstream& toks);

    /**
     * @brief Handles "e" or "eval".
     * 
     * Searches for the best move and displays it and the evaluation.
     */
    void eval(std::istringstream& toks);
};
