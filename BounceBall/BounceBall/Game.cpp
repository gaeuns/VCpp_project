#include "Game.h"

struct Platform 
{
    int x, y, width, height;
};

std::vector<std::vector<Platform>> platforms = 
{
    { {100, 200, 400, 20}, {550, 200, 160, 20}, {770, 200, 100, 20}, {100, 400, 100, 20}, {270, 400, 600, 20}, {550, 50, 50, 20} },
    { {100, 200, 400, 20}, {600, 200, 100, 20}, {100, 400, 100, 20}, {270, 400, 600, 20} }
};

std::vector<std::vector<Platform>> trap =
{
    { {500, 200, 50, 20} }
    //{ {100, 200, 400, 20}, {600, 200, 100, 20}, {100, 400, 100, 20}, {270, 400, 600, 20} }
};

std::vector<std::vector<Platform>> star =
{
    { {850, 170, 20, 20}, {110, 370, 20, 20}, {560, 30, 20, 20} },
    { {110, 370, 20, 20} }
};

void Game::initBall()
{
    x = 100;
    y = 100;
    speedY = 0;
    onGround = false;
    isThread = true;

    star =
    {
        { {850, 170, 20, 20}, {110, 370, 20, 20}, {560, 30, 20, 20} },
        { {110, 370, 20, 20} }
    };
    InvalidateRect(gWnd, NULL, TRUE);

    resumeGame();
}

void getStarPoints(POINT points[], int centerX, int centerY, int radius) {
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
    if (isThread) {
        stopGame();
    }
    if (!isThread)
    {
        isThread = true;
        isJoinble = true;
        ballThread = std::thread(&Game::BallFunction, this);
        threadHandle = ballThread.native_handle();
    }
}

void Game::stopGame()
{
    isThread = false;
    isJoinble = false;

    if (!ballThread.joinable()) {
        MessageBox(gWnd, L"스레드가 이미 종료된 상태입니다.", L"디버깅", MB_OK);
        return;
    }

    if (ballThread.joinable()) {
        try {
            MessageBox(gWnd, L"스레드 종료를 시작합니다.", L"디버깅", MB_OK);
            ballThread.join(); // 스레드 종료 대기
            MessageBox(gWnd, L"스레드가 정상 종료되었습니다.", L"디버깅", MB_OK);
        }
        catch (...) {
            MessageBox(gWnd, L"스레드 종료 중 예외 발생", L"오류", MB_OK);
        }
    }
    else {
        MessageBox(gWnd, L"스레드가 이미 종료된 상태입니다.", L"디버깅", MB_OK);
    }

    ballThread = std::thread();
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

    HPEN myPen = CreatePen(PS_SOLID, 1, RGB(255, 235, 0));
    HBRUSH myBrush = CreateSolidBrush(RGB(255, 235, 0));
    HPEN osPen = (HPEN)SelectObject(hdc, myPen);
    HBRUSH osBrush = (HBRUSH)SelectObject(hdc, myBrush);
    Ellipse(hdc, x - ballRadius, y - ballRadius, x + ballRadius, y + ballRadius);
    SelectObject(hdc, osPen);
    SelectObject(hdc, osBrush);

    myBrush = CreateSolidBrush(RGB(139, 69, 19));
    for (const auto& platform : platforms[currentRound]) 
    {
        RECT platformRect = { platform.x, platform.y, platform.x + platform.width, platform.y + platform.height };
        FillRect(hdc, &platformRect, myBrush);
    }

    myBrush = CreateSolidBrush(RGB(255, 69, 19));
    for (const auto& platform : trap[currentRound])
    {
        RECT trapRect = { platform.x, platform.y, platform.x + platform.width, platform.y + platform.height };
        FillRect(hdc, &trapRect, myBrush);
    }

    myBrush = CreateSolidBrush(RGB(255, 215, 0)); 
    myPen = CreatePen(PS_SOLID, 1, RGB(255, 215, 0));
    osPen = (HPEN)SelectObject(hdc, myPen);
    osBrush = (HBRUSH)SelectObject(hdc, myBrush);

    for (size_t i = 0; i < star[currentRound].size(); ++i) {
        POINT points[10];
        int centerX = star[currentRound][i].x + star[currentRound][i].width / 2;
        int centerY = star[currentRound][i].y + star[currentRound][i].height / 2;
        int radius = star[currentRound][i].width / 2;

        getStarPoints(points, centerX, centerY, radius);
        Polygon(hdc, points, 10);
    }
    SelectObject(hdc, osPen);
    SelectObject(hdc, osBrush);

    DeleteObject(myPen);
    DeleteObject(myBrush);
}

void Game::BallFunction()
{
    while (isThread)
    {
        onGround = false;
        setGravity();
        updatePosition();
        checkCollision();

        if (!isThread) 
        {
            break;
        }

        InvalidateRect(gWnd, NULL, TRUE);

        Sleep(18);
    }
}

void Game::setGravity() 
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
    for (const auto& platform : platforms[currentRound])
    {
        jumpSpeed = -12;
        RECT platformRect = { platform.x, platform.y, platform.x + platform.width, platform.y + platform.height };
        RECT ballRect = getBallRect();
        RECT intersection;

        if (IntersectRect(&intersection, &ballRect, &platformRect))
        {
            y = platformRect.top - ballRadius;
            speedY = jumpSpeed;
            onGround = true;
        }
    }
    for (const auto& platform : trap[currentRound])
    {
        RECT trapRect = { platform.x, platform.y, platform.x + platform.width, platform.y + platform.height };
        RECT ballRect = getBallRect();
        RECT intersectionT;

        if (IntersectRect(&intersectionT, &ballRect, &trapRect))
        {
            jumpSpeed = -20;
            y = trapRect.top - ballRadius;
            speedY = jumpSpeed;
            onGround = true;
        }
    }
    for (size_t i = 0; i < star[currentRound].size(); ++i)
    {
        RECT starRect = { star[currentRound][i].x, star[currentRound][i].y,
                          star[currentRound][i].x + star[currentRound][i].width,
                          star[currentRound][i].y + star[currentRound][i].height };
        RECT ballRect = getBallRect();
        RECT intersectionS;

        if (IntersectRect(&intersectionS, &ballRect, &starRect))
        {
            star[currentRound].erase(star[currentRound].begin() + i);
            if (star[currentRound].empty())
            {
                if (currentRound + 1 >= platforms.size())
                {
                    MessageBox(gWnd, L"모든 스테이지를 클리어했습니다!", L"게임 종료", MB_OK);
                    stopGame();
                    return;
                }
                MessageBox(gWnd, L"모든 별을 수집했습니다! 다음 라운드로 이동합니다.", L"다음 라운드", MB_OK);

                currentRound = (currentRound + 1) % platforms.size();
                starCollected = std::vector<bool>(star[currentRound].size(), false);
                x = 100;
                y = 100;
                speedY = 0;
                onGround = true;
            }
            return;
        }
        if (y > 700)
        {
            x = 200;
            y = 100;
            speedY = 0;
            onGround = true;
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