#pragma once

#include <sstream>

#include "game.h"

/** Command line interface state. */
class CLI {
private:
    /** Game state. */
    Game game;

    /** Is `true` when the CLI was closed, `false` if not. */
    bool isOpen;

public:
    CLI();

    /** Returns `true` if the CLI has not been stopped, `false` if it has. */
    bool is_open() const;

    /** Waits for, reads, and processes the user's next command. */
    void process();

private:
    /**
     * Handles "q" or "quit".
     * 
     * Closes the CLI.
     */
    void quit(std::istringstream& toks);

    /**
     * Handles "h" or "help".
     * 
     * Displays help for the given commands.
     */
    void help(std::istringstream& toks);

    /**
     * Handles "m" or "move".
     * 
     * Reads the move(s) and makes them.
     */
    void move(std::istringstream& toks);

    /**
     * Handles "d" or "display".
     * 
     * Prints the game state.
     */
    void display(std::istringstream& toks);

    /**
     * Handles "p", "pos", or "position".
     * 
     * Sets the position.
     */
    void position(std::istringstream& toks);

    /**
     * Handles "go".
     * 
     * Plays a specified number of moves (default 1).
     */
    void go(std::istringstream& toks);

    /**
     * Handles "e" or "eval".
     * 
     * Searches for the best move and displays it and the evaluation.
     */
    void eval(std::istringstream& toks);
};
