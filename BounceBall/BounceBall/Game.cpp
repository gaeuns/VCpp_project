#include "Game.h"

void Game::startGame(HWND hWnd)
{
    isThread = true;
    ballThread = std::thread(&Game::BallFunction, this);
}

void Game::stopGame() 
{
    isThread = false;
    if (ballThread.joinable()) 
        ballThread.join();
}

struct Platform 
{
    int x, y, width, height;
};

std::vector<std::vector<Platform>> platforms = 
{
    { {100, 200, 600, 20}, {770, 200, 100, 20}, {100, 400, 100, 20}, {270, 400, 600, 20} }
};

std::vector<std::vector<Platform>> star =
{
    { {850, 170, 20, 20} }
};

void Game::createGame(HWND hWnd, HDC hdc)
{
    gWnd = hWnd;
    ghdc = hdc;

    HPEN ballPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
    HBRUSH ballBrush = CreateSolidBrush(RGB(255, 255, 0));
    HPEN osPen = (HPEN)SelectObject(hdc, ballPen);
    HBRUSH osBrush = (HBRUSH)SelectObject(hdc, ballBrush);
    Ellipse(hdc, x - ballRadius, y - ballRadius, x + ballRadius, y + ballRadius);
    SelectObject(hdc, osPen);
    SelectObject(hdc, osBrush);
    DeleteObject(ballPen);
    DeleteObject(ballBrush);

    HBRUSH platformBrush = CreateSolidBrush(RGB(139, 69, 19));
    for (const auto& platform : platforms[currentRound]) 
    {
        RECT platformRect = { platform.x, platform.y, platform.x + platform.width, platform.y + platform.height };
        FillRect(hdc, &platformRect, platformBrush);
    }
    DeleteObject(platformBrush);
}

void Game::BallFunction() 
{
    while (isThread)
    {
        applyGravity();
        updatePosition();
        checkCollision();

        InvalidateRect(gWnd, NULL, TRUE);

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void Game::applyGravity() 
{
    if (!onGround)
        speedY += gravity;
}

void Game::updatePosition() 
{
    y += speedY;
}

RECT Game::getRect() const 
{
    return RECT{ x - ballRadius, y - ballRadius, x + ballRadius, y + ballRadius };
}

void Game::checkCollision() 
{
    onGround = false;
    for (const auto& platform : platforms[currentRound]) 
    {
        RECT platformRect = { platform.x, platform.y, platform.x + platform.width, platform.y + platform.height };
        RECT ballRect = getRect();
        RECT intersection;

        if (IntersectRect(&intersection, &ballRect, &platformRect)) 
        {
            y = platformRect.top - ballRadius;
            speedY = jumpSpeed;
            onGround = true;
        }
        if (ballRect.top > 700)
        {
            createGame(gWnd, ghdc);
            InvalidateRect(gWnd, NULL, TRUE);
        }
    }
}

void Game::moveLeft() 
{
    x -= speedX;
    if (x - ballRadius < 0) 
        x = ballRadius;
}

void Game::moveRight() 
{
    x += speedX;
    if (x + ballRadius > 1000) 
        x = 1000 - ballRadius;
}