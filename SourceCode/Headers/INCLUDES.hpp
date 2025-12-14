#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <random>

#include "../../FileAPI/FileHandler.hpp"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800

#define PADDLE_SPEED 300
#define PADDLE_WIDTH 150
#define PADDLE_HEIGTH 30


#define BALL_SPEED_INIT 282.84// √(200^2 + 200^2); aka, pythagoream theorem
#define MAX_BALL_SPEED 300
#define BALL_RADIUS 8

#define BRICK_ROW_NUM 6
#define BRICK_COL_NUM 10
#define BRICK_WIDTH 90
#define BRICK_HEIGTH 50


enum class GAME_STATE{
    Start,
    Playing,
    Gameover
};