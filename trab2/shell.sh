#!/bin/bash

# João Victor Dell Agli Floriano - 10799783
# Sistemas Operacionais - 2022.1

find $PWD -name '*.sh' | while read -r line; do $(bash shebang.sh "$line"); done