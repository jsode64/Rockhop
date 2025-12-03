#include "cli.h"
#include "def.h"

i32 main() {
    CLI cli;
    while (cli.is_open())
        cli.process();
}
