#!/bin/bash

# CHANGE THIS TO YOUR FOLDER
SRC=~/Documents/Scripts/Arduino/IMECX/LIFT/libraries

find "$SRC" -type f -name "*.h" | while read -r header; do
    name=$(basename "$header")

    # search everywhere except the source directory itself
    matches=$(find ~ -type f -name "$name" -not -path "$SRC/*" 2>/dev/null)

    found=0

    for target in $matches; do
        found=1

        # only replace if different
        if ! cmp -s "$header" "$target"; then
            echo "Updating $target"
            cp "$header" "$target"
        fi
    done

    [ "$found" -eq 0 ] && echo "No match for $name"
done