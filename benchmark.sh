#!/bin/bash

echo -e "go depth 20 for 99\nq" | build/rockhop

# NOTE: All searches are done with a depth of 20

# Move odrdering only first depth:  14.416s
# Move ordering every depth:        5.762s
# Optimized move sorting:           4.743s
