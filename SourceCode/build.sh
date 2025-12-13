#!/usr/bin/env bash

set -e  # stop on first error

echo "Building Breakout..."

g++ Main.cpp Game.cpp Ball.cpp Paddle.cpp Brick.cpp \
    -o ../EXE/Breakout \
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

echo "Build successful!"
