#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <random>
#include <memory>

#include "../../FileAPI/FileHandler.hpp"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800

#define PADDLE_SPEED 375
#define PADDLE_WIDTH 150
#define PADDLE_HEIGTH 30


#define BALL_SPEED_INIT 300
#define MAX_BALL_SPEED 600
#define BALL_RADIUS 8
//#define BALL_SPEED_MULT 1.2

#define BRICK_ROW_NUM 6
#define BRICK_COL_NUM 10
#define BRICK_WIDTH 90
#define BRICK_HEIGTH 50


enum class GAME_STATE{
    Start,
    Playing,
    LevelCleared,
    Gameover
};

enum BRICK_TYPE{
    NORMAL
};