#include "game.h"

int32_t main() {
    Game game;

    while (!game.is_over()) {
        game.display();
        game.move();
    }
}
