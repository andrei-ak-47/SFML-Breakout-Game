#include "../Headers/Game.hpp"

void Game::run() {
    FileAPI::FileHandler file("../Logs.txt", true);
    file.appendLine("NEW GAME STARTED");
    // Game loop
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        window.clear(sf::Color::Black);

        // Event handling
        while (const std::optional eventOpt = window.pollEvent()) {
            const sf::Event& event = *eventOpt;
            if (event.is<sf::Event::Closed>()) {
                window.close();
            }
        }

        switch (GameState) {
            case GAME_STATE::Start: {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                    //Reposition all pieces as needed and change GameState
                    ResetPositions();
                    GameState = GAME_STATE::Playing;
                }

                sf::Text StartText(GlobalFont, "Press Space to Start\n", 30);
                StartText.setOrigin(StartText.getLocalBounds().getCenter());//Set the origin from top-left corner to middle
                StartText.setPosition({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f});
                window.draw(StartText);
                break;
            }

            case GAME_STATE::Playing: {
                // Automatically restart level if all bricks destroyed
                file.appendLine("BALL VELOCITY: " + std::to_string(ball.GetVelocity().x) + ", " + std::to_string(ball.GetVelocity().y));
                if (bricksNum <= 0) {
                    //Put all bricks back
                    Bricks_init();
                    //Set speed before reseting position so that correct measurment in the ResetPositions
                    ball.MultiplySpeed(1.1);
                    ResetPositions();//Reset all positions and manage velocity change
                }

                // Paddle input
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                    paddle.MoveRight();
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                    paddle.MoveLeft();
                else//If nothing pressed, set velocity of paddle to 0
                    paddle.SetVelocityX(0);

                paddle.Update(deltaTime);

                ball.Update(deltaTime);
                CheckCollisions();

                Render();
                break;
            }

            case GAME_STATE::Gameover: {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                    //Set speed before reseting position so that correct measurment in the ResetPositions
                    ball.SetSpeedMultiplier(1);//Set speed back to normal
                    ResetPositions();//Reset all positions and manage velocity
                    Bricks_init();//reset all bricks

                    lives = 3;
                    score = 0;
                    GameState = GAME_STATE::Playing;
                }

                sf::Text GameOver(GlobalFont, "Game Over\nFinal Score: " + std::to_string(score), 30);
                GameOver.setOrigin(GameOver.getLocalBounds().getCenter());//Sets origin from top-left to middle
                GameOver.setPosition({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f});
                window.draw(GameOver);
                break;
            }
        }

        window.display();
    }
}

Game::Game() :
    window(sf::VideoMode({1000, 800}), "Breakout", sf::Style::Default),
    ball({350, 400}, sf::Color::White, BALL_RADIUS),
    paddle({300, 700}, sf::Color::White, {PADDLE_WIDTH, PADDLE_HEIGTH}),
    scoreText(GlobalFont, "", 30),
    LivesText(GlobalFont, "", 30),
    SpeedText(GlobalFont, "", 30),
    paddleSound(paddleBuffer),
    wallSound(wallBuffer),
    brickSound(brickBuffer),
    diedSound(diedBuffer),
    lives(3),
    score(0){

    //Open all Files
    Bricks_init();
    if (!GlobalFont.openFromFile("../../Assets/Font/ARLRDBD.TTF"))
        std::cout << "[ERROR]: Failed to load font\n";

    if (!brickBuffer.loadFromFile("../../Assets/SFX/BrickHit.mp3"))
        std::cout << "[ERROR]: Failed to load brickBuffer SFX\n";
    brickSound.setBuffer(brickBuffer);

    if (!paddleBuffer.loadFromFile("../../Assets/SFX/PaddleHit.mp3"))
        std::cout << "[ERROR]: Failed to load paddleBuffer SFX\n";
    paddleSound.setBuffer(paddleBuffer);

    if (!wallBuffer.loadFromFile("../../Assets/SFX/WallHit.wav"))
        std::cout << "[ERROR]: Failed to load wallBuffer SFX\n";
    wallSound.setBuffer(wallBuffer);

    if (!diedBuffer.loadFromFile("../../Assets/SFX/Died.wav"))
        std::cout << "[ERROR]: Failed to load diedBuffer SFX\n";
    diedSound.setBuffer(diedBuffer);

    window.setFramerateLimit(144);
    window.setVerticalSyncEnabled(true);
}

void Game::ResetPositions() {

    paddle.SetPosition({300, 700});//Set paddle position back to start

    ball.SetPosition({350, 400});//Set ball positions back to start
    ball.ChooseDirection();//Chose the random direction to start and calculate X and Y vector movements based of random angle and velocity
}

void Game::Bricks_init() {
    /*
        Makes the brick vector
        Num Of Rows: BRICK_ROW_NUM
        Num of Cols: BRICK_COL_NUM
        Colors are determined by row

        Colors:
            Blue
            Red
            Yellow
            Green
    */
    std::vector<sf::Color> colors = {sf::Color::Blue, sf::Color::Red, sf::Color::Yellow, sf::Color::Green};

    float padding = 5.f;//small distance between blocks
    float offsetX = 25.f;//small whole movement to the right from the left side for "padding"
    float offsetY = 50.f;//same as offset X

    bricks.clear();
    bricksNum = 0;

    for (int i = 0; i < BRICK_ROW_NUM; i++) {
        std::vector<Brick> row;
        for (int j = 0; j < BRICK_COL_NUM; j++) {
            float x = j * (BRICK_WIDTH + padding) + offsetX;
            float y = i * (BRICK_HEIGTH + padding) + offsetY;
            row.push_back(Brick({x, y}, colors[i % colors.size()], {BRICK_WIDTH, BRICK_HEIGTH}));
            bricksNum++;
        }
        bricks.push_back(row);
    }
}

void Game::CheckCollisions() {
    const float PENETRATION_OFFSET = 0.1f; // tiny offset to prevent sticking
    const float MAX_ANGLE = 60.f;
    auto ballBounds = ball.GetBounds();
    auto ballPos = ball.GetPosition();
    auto ballVel = ball.GetVelocity();

    /* ---------------- Ball & Paddle ---------------- */
    auto paddleBounds = paddle.GetBounds();
    if (ballBounds.findIntersection(paddleBounds)) {
        //Only change velocity if moving down
        //Prevents wall-sticking bugs
        if (ballVel.y > 0) {

            /*Step One: Calculate where ball hit*/

            float BallCenterX = ballBounds.getCenter().x;
            float PaddleCenterX = paddleBounds.getCenter().x;
            float BallVelocity = BALL_SPEED_INIT * ball.GetSpeedMultiplier();

            float DistancePX = BallCenterX - PaddleCenterX;

            /*Step Two: Convert hit position to scale [-1, 1]*/

            float DistanceScale = (DistancePX * 2) / PADDLE_WIDTH;

            /*Step Three: Calculate X and Y velocities using trigonometry*/
            float AngleDeg = MAX_ANGLE * DistanceScale;
            float AngleRad = AngleDeg * M_PI / 180.f;

            float VelocityX;
            float VelocityY;

            if(AngleRad < 0){

                VelocityX = -(BallVelocity * std::sin(std::abs(AngleRad)));
            }
            else{
                VelocityX = (BallVelocity * std::sin(std::abs(AngleRad)));
            }   

            VelocityY = -(BallVelocity * std::cos(std::abs(AngleRad)));


            ball.SetVelocityX(VelocityX);
            ball.SetVelocityY(VelocityY);
        }
    }

    /* ---------------- Ball & Bricks ---------------- */
    bool brickHit = false;
    for (auto& row : bricks) {
        for (auto& brick : row) {
            if (brick.IsBroken()) continue;

            auto brickBounds = brick.GetBounds();
            if (ballBounds.findIntersection(brickBounds)) {
                std::cout << "BRICK INTERSECTION\n";
                brick.Break();
                brickSound.play();
                score += 100;
                bricksNum--;
                brickHit = true;

                // Calculate overlap
                float overlapLeft   = (ballBounds.position.x + ballBounds.size.x) - brickBounds.position.x;
                float overlapRight  = (brickBounds.position.x + brickBounds.size.x) - ballBounds.position.x;
                float overlapTop    = (ballBounds.position.y + ballBounds.size.y) - brickBounds.position.y;
                float overlapBottom = (brickBounds.position.y + brickBounds.size.y) - ballBounds.position.y;

                float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

                if (minOverlap == overlapTop)
                    ball.Move({0.f, -overlapTop - PENETRATION_OFFSET});
                else if (minOverlap == overlapBottom)
                    ball.Move({0.f, overlapBottom + PENETRATION_OFFSET});
                else if (minOverlap == overlapLeft)
                    ball.Move({-overlapLeft - PENETRATION_OFFSET, 0.f});
                else
                    ball.Move({overlapRight + PENETRATION_OFFSET, 0.f});

                // Flip appropriate velocity
                if (minOverlap == overlapLeft || minOverlap == overlapRight)
                    ball.FlipOnX();
                else
                    ball.FlipOnY();

                break; // stop checking bricks for this frame
            }
        }
        if (brickHit) break;
    }

    /* ---------------- Ball & Walls ---------------- */
    // Right wall
    if (ballPos.x + ballBounds.size.x > WINDOW_WIDTH) {
        //Move back to fit inside border
        ball.Move({WINDOW_WIDTH - (ballPos.x + ballBounds.size.x), 0.f});
        //Only flip velocity if moving towards the wall
        //Prevents wall-sticking bugs
        if (ballVel.x > 0) {
            std::cout << "WALL RIGHT INTERSECTION\n";
            ball.FlipOnX();
            wallSound.play();
        }
    }

    // Left wall
    if (ballPos.x < 0) {
        ball.Move({-ballPos.x, 0.f});
        //Only flip velocity if moving towards the wall
        //Prevents wall-sticking bugs
        if (ballVel.x < 0) {
            std::cout << "WALL LEFT INTERSECTION\n";
            ball.FlipOnX();
            wallSound.play();
        }
    }

    // Bottom wall
    if (ballPos.y + ballBounds.size.y > WINDOW_HEIGHT) {//Fell through, you died
        diedSound.play();
        lives--;
        GameState = (lives > 0) ? GAME_STATE::Start : GAME_STATE::Gameover;
    }

    // Top wall
    if (ballPos.y < 0) {
        ball.Move({0.f, -ballPos.y});
        //Only flip velocity if moving towards the wall
        //Prevents wall-sticking bugs
        if (ballVel.y < 0) {
            std::cout << "WALL TOP INTERSECTION\n";
            ball.FlipOnY();
            wallSound.play();
        }
    }
}


void Game::Render() {
    /* Paddle, Ball, Bricks */
    paddle.Draw(window);
    ball.Draw(window);

    for (auto& row : bricks)
        for (auto& brick : row)
            if (!brick.IsBroken())
                brick.Draw(window);

    /* Score & Lives & Speed */
    scoreText.setString("SCORE: " + std::to_string(score));
    scoreText.setPosition({WINDOW_WIDTH / 2.f - scoreText.getGlobalBounds().size.x / 2.f, 10});
    scoreText.setFillColor(sf::Color::White);
    window.draw(scoreText);

    LivesText.setString("LIVES: " + std::to_string(lives));
    LivesText.setPosition({0, 0});
    LivesText.setFillColor(sf::Color::White);
    window.draw(LivesText);

    SpeedText.setString("Speed: " + std::to_string(ball.GetSpeedMultiplier()) + 'x');
    SpeedText.setPosition({WINDOW_WIDTH - SpeedText.getLocalBounds().size.x - 10, 0});
    SpeedText.setFillColor(sf::Color::White);
    window.draw(SpeedText);
}
