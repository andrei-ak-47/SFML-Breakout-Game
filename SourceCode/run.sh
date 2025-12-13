#!/usr/bin/env bash

set -e

EXE="../EXE/Breakout"

if [[ ! -f "$EXE" ]]; then
    echo "Executable not found. Building first..."
    ./build.sh
fi

echo "Running Breakout..."
"$EXE"
