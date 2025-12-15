#include "../Headers/Game.hpp"

void Game::run() {
    //FileAPI::FileHandler file("../Logs.txt", true);
    //file.appendLine("NEW GAME STARTED");
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
                std::cout << "Ball Vel: X: ";
                std::cout << ball.GetVelocity().x << ", Y: " << ball.GetVelocity().y << '\n';
                if (bricksNum <= 0) {
                    GameState = GAME_STATE::LevelCleared;
                }

                // Paddle input
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                    paddle.MoveRight();
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                    paddle.MoveLeft();
                else//If nothing pressed, set velocity of paddle to 0
                    paddle.SetVelocityX(0);

                paddle.Update(deltaTime);

                int SubStepCount = 5;
                for(int i = 0; i < SubStepCount; i++){
                    ball.Update(deltaTime / SubStepCount);
                    CheckCollisions();

                    if(GameState != GAME_STATE::Playing) break;
                }

                Render();
                break;
            }
            case GAME_STATE::LevelCleared:{
                //When level is cleared
                if (++CurrentLvl >= LevelPaths.size()) {
                    CurrentLvl = 0; // or go to GameOver/Win screen
                }
                LoadLevel(CurrentLvl);

                ball.IncreaseSpeed();

                ResetPositions();
                GameState = GAME_STATE::Start;
                break;
            }
            case GAME_STATE::Gameover: {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                    //Set speed before reseting position so that correct measurment in the ResetPositions
                    ball.ResetSpeed();//Set speed back to normal
                    ResetPositions();//Reset all positions and manage velocity

                    CurrentLvl = 0;
                    LoadLevel(CurrentLvl);//reset all bricks

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
    ball("../../Assets/Sprites/Ball.png", {350, 400}),
    paddle("../../Assets/Sprites/Paddle.png", {300, 700}),
    scoreText(GlobalFont, "", 30),
    LivesText(GlobalFont, "", 30),
    SpeedText(GlobalFont, "", 30),
    paddleSound(paddleBuffer),
    wallSound(wallBuffer),
    brickSound(brickBuffer),
    diedSound(diedBuffer),
    lives(3),
    score(0),
    CurrentLvl(0){

    LoadFiles();


    LoadLevel(CurrentLvl);

    window.setFramerateLimit(144);
    window.setVerticalSyncEnabled(true);
}

void Game::LoadFiles(){
    //Open all Files
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

    brickTextures[0] = {sf::Texture("../../Assets/Sprites/BlueBrick.png")};
    brickTextures[1] = {sf::Texture("../../Assets/Sprites/RedBrick.png")};
    brickTextures[2] = {sf::Texture("../../Assets/Sprites/YellowBrick.png")};
    brickTextures[3] = {sf::Texture("../../Assets/Sprites/GreenBrick.png")};

    LevelPaths.push_back("../../Assets/Levels/Level1.txt");
    LevelPaths.push_back("../../Assets/Levels/Level2.txt");
    LevelPaths.push_back("../../Assets/Levels/Level3.txt");
    LevelPaths.push_back("../../Assets/Levels/Level4.txt");
    LevelPaths.push_back("../../Assets/Levels/Level5.txt");
}

void Game::ResetPositions() {

    //paddle.SetPosition({300, 700});//Set paddle position back to start
    float paddleX = WINDOW_WIDTH / 2.f - PADDLE_WIDTH / 2.f;
    float paddleY = WINDOW_HEIGHT - 100.f; // tweak this value

    paddle.SetPosition({paddleX, paddleY});

    float ballX = WINDOW_WIDTH / 2.f - BALL_RADIUS;
    float ballY = paddleY - BALL_RADIUS * 2.f - 5.f;

    ball.SetPosition({ballX, ballY});
    
    ball.SetVelocityX(0);
    ball.SetVelocityY(ball.GetSpeed());//Set the ball going fully down
}

void Game::LoadLevel(int LevelID){

    bricks.clear();
    bricksNum = 0;

    float padding = 5.f;
    float offsetX = 25.f;
    float offsetY = 50.f;

    std::string FilePath = LevelPaths[LevelID];

    FileAPI::FileHandler file(FilePath, true);

    for(int i = 0; i < BRICK_ROW_NUM; i++){
        std::string FileRow = file.readLine(true);
        //std::vector<std::unique_ptr<Brick>> row;

        if (FileRow.empty()) break;

        for(int j = 0; j < BRICK_COL_NUM && j < FileRow.size(); j++){

            float x = j * (BRICK_WIDTH + padding) + offsetX;
            float y = i * (BRICK_HEIGTH + padding) + offsetY;

            switch(FileRow[j]){
                case '1':{
                    //std::string fileName = ColorFileNames[i % 4];
                    bricks.emplace_back(std::make_unique<Brick>(brickTextures[i % brickTextures.size()], sf::Vector2f{x, y}, BRICK_TYPE::NORMAL));
                    bricksNum++;
                    break;
                }
            }

        }
        //bricks.emplace_back(std::move(row));
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
            paddleSound.play();

            /*Step One: Calculate where ball hit*/

            float BallCenterX = ballBounds.getCenter().x;
            float PaddleCenterX = paddleBounds.getCenter().x;
            float BallVelocity = ball.GetSpeed();

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
    for (auto it = bricks.begin(); it != bricks.end(); ++it) {
        auto& brick = *it;
        auto brickBounds = brick->GetBounds();
        
        if (ballBounds.findIntersection(brickBounds)) {
            std::cout << "BRICK INTERSECTION\n";

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

            it = bricks.erase(it); // erase returns the next valid iterator
            break; // stop checking bricks for this frame
        }
    }


    /* ---------------- Ball & Walls ---------------- */
    // Right wall
    if (ballPos.x + ballBounds.size.x > WINDOW_WIDTH) {
        //Move back to fit inside border
        ball.Move({WINDOW_WIDTH - (ballPos.x + ballBounds.size.x), 0.f});
        //Only flip velocity if moving towards the wall
        //Prevents wall-sticking bugs
        if (ballVel.x > 0) {
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
            ball.FlipOnY();
            wallSound.play();
        }
    }
}

void Game::Render() {
    /* Paddle, Ball, Bricks */
    paddle.Draw(window);
    ball.Draw(window);

    for (auto& brick : bricks){
        brick->Draw(window);
    }
           

    /* Score & Lives & Speed */
    scoreText.setString("SCORE: " + std::to_string(score));
    scoreText.setPosition({WINDOW_WIDTH / 2.f - scoreText.getGlobalBounds().size.x / 2.f, 10});
    scoreText.setFillColor(sf::Color::White);
    window.draw(scoreText);

    LivesText.setString("LIVES: " + std::to_string(lives));
    LivesText.setPosition({0, 0});
    LivesText.setFillColor(sf::Color::White);
    window.draw(LivesText);

    SpeedText.setString("Speed: " + std::to_string(ball.GetSpeed()) + "px/s");
    SpeedText.setPosition({WINDOW_WIDTH - SpeedText.getLocalBounds().size.x - 10, 0});
    SpeedText.setFillColor(sf::Color::White);
    window.draw(SpeedText);
}
