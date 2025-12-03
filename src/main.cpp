#include <print>

#include "def.h"
#include "game.h"

i32 main() {
    Game game;

    while (!game.is_over()) {
        game.display();
        game.move();
    }

    game.display();
}
