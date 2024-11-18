#include "Game.h"

struct Platform 
{
    int x, y, width, height;
};

std::vector<std::vector<Platform>> platforms = 
{
    { {100, 200, 600, 20}, {770, 200, 100, 20}, {100, 400, 100, 20}, {270, 400, 600, 20} }
    //{ {100, 200, 400, 20}, {600, 200, 100, 20}, {100, 400, 100, 20}, {270, 400, 600, 20} }
};

std::vector<std::vector<Platform>> star =
{
    //{ {850, 170, 20, 20}, {110, 370, 20, 20} },
    { {110, 370, 20, 20} }
};

void Game::initBall()
{
    x = 200;
    y = 100;
    speedY = 0;
    onGround = true;

    star =
    {
        //{ {850, 170, 20, 20}, {110, 370, 20, 20} },
        { {110, 370, 20, 20} }
    };

    createGame(gWnd, ghdc);
}

void GetStarPoints(POINT points[], int centerX, int centerY, int radius) {
    const double PI = 3.14159265358979323846;
    int numPoints = 10;

    for (int i = 0; i < numPoints; ++i) {
        double angle = PI / 2 + i * PI / 5;
        double r = (i % 2 == 0) ? radius : radius / 2;
        points[i].x = static_cast<LONG>(centerX + r * cos(angle));
        points[i].y = static_cast<LONG>(centerY - r * sin(angle));
    }
}

void Game::startGame(HWND hWnd)
{
    if (!isThread)
    {
        isThread = true;
        ballThread = std::thread(&Game::BallFunction, this);
        threadHandle = ballThread.native_handle();
    }
}

void Game::stopGame()
{
    isThread = false;
    if (ballThread.joinable())
        ballThread.join();
    gWnd = nullptr;
    ghdc = nullptr;
}

void Game::suspendGame()
{
    SuspendThread(threadHandle);
}

void Game::resumeGame()
{
    ResumeThread(threadHandle);
}

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

    HBRUSH starBrush = CreateSolidBrush(RGB(255, 215, 0)); 
    HPEN starPen = CreatePen(PS_SOLID, 1, RGB(255, 215, 0));
    HPEN oldPen = (HPEN)SelectObject(hdc, starPen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, starBrush);

    for (size_t i = 0; i < star[currentRound].size(); ++i) {
        POINT points[10];
        int centerX = star[currentRound][i].x + star[currentRound][i].width / 2;
        int centerY = star[currentRound][i].y + star[currentRound][i].height / 2;
        int radius = star[currentRound][i].width / 2;

        GetStarPoints(points, centerX, centerY, radius);
        Polygon(hdc, points, 10);
    }

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(starPen);
    DeleteObject(starBrush);
}

void Game::BallFunction() 
{
    while (isThread)
    {
        applyGravity();
        updatePosition();
        checkCollision();

        InvalidateRect(gWnd, NULL, TRUE);

        Sleep(18);
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

RECT Game::getBallRect() const 
{
    return RECT{ x - ballRadius, y - ballRadius, x + ballRadius, y + ballRadius };
}

void Game::checkCollision() 
{
    onGround = false;
    for (const auto& platform : platforms[currentRound])
    {
        RECT platformRect = { platform.x, platform.y, platform.x + platform.width, platform.y + platform.height };
        RECT ballRect = getBallRect();
        RECT intersection;

        if (IntersectRect(&intersection, &ballRect, &platformRect))
        {
            y = platformRect.top - ballRadius;
            speedY = jumpSpeed;
            onGround = true;
        }
        if (y > 700)
        {
            x = 200;
            y = 100;
            speedY = 0;
            onGround = true;
        }
    }
    for (size_t i = 0; i < star[currentRound].size(); ++i) {
        RECT starRect = { star[currentRound][i].x, star[currentRound][i].y,
                          star[currentRound][i].x + star[currentRound][i].width,
                          star[currentRound][i].y + star[currentRound][i].height };
        RECT ballRect = getBallRect();
        RECT intersection;

        if (IntersectRect(&intersection, &ballRect, &starRect)) {
            star[currentRound].erase(star[currentRound].begin() + i);
            if (star[currentRound].empty()) {
                if (currentRound+1 >= platforms.size())
                {
                    MessageBox(gWnd, L"모든 스테이지를 클리어했습니다!", L"게임 종료", MB_OK);
                    stopGame();
                    PostQuitMessage(0);
                    return;
                }
                if (currentRound >= star.size()) {
                    MessageBox(gWnd, L"별 데이터가 없습니다!", L"에러", MB_OK);
                    stopGame();
                    PostQuitMessage(0);
                    return;
                }
                MessageBox(gWnd, L"모든 별을 수집했습니다! 다음 라운드로 이동합니다.", L"다음 라운드", MB_OK);

                currentRound = (currentRound + 1) % platforms.size();
                starCollected = std::vector<bool>(star[currentRound].size(), false);
                x = 200;
                y = 100;
                speedY = 0;
                onGround = true;
            }
            break;
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