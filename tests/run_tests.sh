#!/bin/bash

DIRS=()
CURRENT_PATH="$(realpath $PWD)"

for FILE in "$CURRENT_PATH"/*; do
    # echo "$FILE"
    [[ -d "$FILE" ]] && DIRS+=("$FILE")
done

for TEST in "${DIRS[@]}"; do
    TEST_CMD="g++ -std=c++11 $TEST/test.cc -g -Wall -o $TEST/test"
    # echo -e "${TEST##*/} $($TEST_CMD && $TEST/test)"
    RESULT+="${TEST##*/} $($TEST_CMD && $TEST/test)\n"
done

# Cleanup executables
for TEST in "${DIRS[@]}"; do
    rm -rf "$TEST/test"
done

echo -ne "$RESULT" | column -t

