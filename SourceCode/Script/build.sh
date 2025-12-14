#!/usr/bin/env bash

set -e  # stop on first error

echo "Building Breakout..."

g++ ../CPP/Main.cpp ../CPP/Game.cpp ../CPP/Ball.cpp ../CPP/Paddle.cpp ../CPP/Brick.cpp \
    -o ../../EXE/Breakout \
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

echo "Build successful!"
