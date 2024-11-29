#include "Game.h"

struct Platform 
{
    int x, y, width, height;
};

std::vector<std::vector<Platform>> platforms = 
{
    { {30, 250, 370, 25}, {550, 250, 200, 25},  {100, 450, 100, 25}, {270, 450, 600, 25}, {450, 100, 50, 25} },
    { {100, 200, 100, 25}, {500, 200, 100, 25}, {400, 200, 100, 25}, {630, 400, 50, 25}, {700, 250, 100, 25} },
    { {100, 230, 300, 25}, {550, 230, 200, 25},  {100, 400, 100, 25}, {270, 400, 600, 25}, {450, 80, 50, 25} }

};

std::vector<std::vector<Platform>> trap =
{
    { {400, 250, 50, 25} },
    { {230, 300, 40, 25}, {300, 200, 40, 25}, {730, 450, 50, 25} },
    { {400, 230, 50, 25} }
};

std::vector<std::vector<Platform>> star =
{
    { {700, 220, 30, 30}, {110, 420, 30, 30}, {460, 30, 30, 30} },
    { {230, 30, 30, 30}, {780, 500, 30, 30} },
    { {700, 200, 30, 30}, {110, 370, 30, 30}, {460, 20, 30, 30} }
};

std::vector<std::vector<Platform>> bombs = {
    { {180, 0, 30, 30}, {310, -150, 30, 30}, {450, -100, 30, 30}, {750, -200, 30, 30} },
    { {50, -50, 30, 30}, {180, 0, 30, 30}, {310, -150, 30, 30}, {450, -100, 30, 30}, {750, -200, 30, 30} },
    { {50, -50, 30, 30}, {180, 0, 30, 30}, {310, -150, 30, 30}, {450, -100, 30, 30}, {550, -50, 30, 30}, {750, -200, 30, 30} }
};

// ���� �ʱ�ȭ �Լ� ( ȹ���ߴ� ���� ��� �ʱ�ȭ )
void Game::initBall()
{
    x = 150;
    y = 100;
    speedY = 0;
    onGround = false;
    isThread = true;

    star =
    {
        { {700, 200, 30, 30}, {110, 370, 30, 30}, {460, 20, 30, 30} },
        { {230, 30, 30, 30}, {780, 500, 30, 30} },
        { {700, 200, 30, 30}, {110, 370, 30, 30}, {460, 20, 30, 30} }
    };
    bombs =
    {
        { {180, 0, 30, 30}, {310, -150, 30, 30}, {450, -100, 30, 30}, {750, -200, 30, 30} },
        { {50, -50, 30, 30}, {180, 0, 30, 30}, {310, -150, 30, 30}, {450, -100, 30, 30}, {750, -200, 30, 30} },
        { {50, -50, 30, 30}, {180, 0, 30, 30}, {310, -150, 30, 30}, {450, -100, 30, 30}, {550, -50, 30, 30}, {750, -200, 30, 30} }
    };

    InvalidateRect(gWnd, NULL, TRUE);

    resumeGame();
}

// �� ������ ���ϴ� �Լ�
void getStarPoints(POINT points[], int centerX, int centerY, int radius) {
    const double PI = 3.14159265358979323846;
    int numPoints = 10;

    for (int i = 0; i < numPoints; ++i) 
    {
        double angle = PI / 2 + i * PI / 5;
        double r = (i % 2 == 0) ? radius : radius / 2;
        points[i].x = static_cast<LONG>(centerX + r * cos(angle));
        points[i].y = static_cast<LONG>(centerY - r * sin(angle));
    }
}

// ������ ���� �Լ�
void Game::startGame(HWND hWnd)
{
    if (!isThread)
    {
        isThread = true;
        ballThread = std::thread(&Game::ballFunction, this);
        threadHandle = ballThread.native_handle();
    }
}

// ������ ���� �Լ�
void Game::stopGame()
{
    isThread = false;

    if (ballThread.joinable())
    {
        ballThread.join();
    }
}

// ������ �Ͻ����� �Լ�
void Game::suspendGame()
{
    SuspendThread(threadHandle);
}

// ������ ��� �Լ�
void Game::resumeGame()
{
    ResumeThread(threadHandle);
}

// ���� ���� �� ��ü�� �׸��� �Լ�
void Game::createGame(HWND hWnd, HDC hdc, int red, int green, int blue)
{
    gWnd = hWnd;
    ghdc = hdc;

    // ��
    myPen = CreatePen(PS_SOLID, 1, RGB(red, green, blue));
    myBrush = CreateSolidBrush(RGB(red, green, blue));
    osPen = (HPEN)SelectObject(hdc, myPen);
    osBrush = (HBRUSH)SelectObject(hdc, myBrush);

    Ellipse(hdc, x - ballRadius, y - ballRadius, x + ballRadius, y + ballRadius);

    SelectObject(hdc, osPen);
    SelectObject(hdc, osBrush);
    DeleteObject(myPen);
    DeleteObject(myBrush);

    // �÷���
    myBrush = CreateSolidBrush(RGB(139, 69, 19)); 
    for (const auto& platform : platforms[currentRound])
    {
        RECT platformRect = { platform.x, platform.y, platform.x + platform.width, platform.y + platform.height };
        FillRect(hdc, &platformRect, myBrush);
    }
    DeleteObject(myBrush); 

    // ���� �÷���
    myBrush = CreateSolidBrush(RGB(255, 69, 19));
    for (const auto& platform : trap[currentRound])
    {
        RECT trapRect = { platform.x, platform.y, platform.x + platform.width, platform.y + platform.height };
        FillRect(hdc, &trapRect, myBrush);
    }
    DeleteObject(myBrush); 

    // ��
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

    // ��ź
    if (gameMode == 1) 
    {
        myPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        myBrush = CreateSolidBrush(RGB(0, 0, 0));
        osPen = (HPEN)SelectObject(hdc, myPen);
        osBrush = (HBRUSH)SelectObject(hdc, myBrush);

        for (const auto& bomb : bombs[currentRound]) 
        {
            Ellipse(hdc, bomb.x, bomb.y, bomb.x + bomb.width, bomb.y + bomb.height);
        }
        SelectObject(hdc, osPen);
        SelectObject(hdc, osBrush);
        DeleteObject(myPen);
        DeleteObject(myBrush);
    }
}

// �����尡 ���۽�Ű�� �Լ�
void Game::ballFunction()
{
    while (isThread)
    {
        onGround = false;

        if (!onGround)
        {
            speedY += gravity;
        }
        y += speedY;

        checkCollision();

        if (gameMode == 1)
        {
            moveBomb();
            checkCollisionBomb();
        }
        if (y > 650)
        {
            x = 150;
            y = 100;
            speedY = 0;
            onGround = true;
        }
        PostMessage(gWnd, WM_USER + 1, 0, 0);
        Sleep(18);
    }
}

// ��ź �̵� �Լ�
void Game::moveBomb()
{
    for (auto& bomb : bombs[currentRound]) 
    {
        bomb.y += 10; 
        if (bomb.y > 650) 
        { 
            bomb.y = -50;
            bomb.x = rand() % 800;
        }
    }
}

// ��ź�� �¾Ҵ��� Ȯ���ϴ� �Լ�
void Game::checkCollisionBomb()
{
    for (const auto& bomb : bombs[currentRound]) 
    {
        RECT bombRect = { bomb.x, bomb.y, bomb.x + bomb.width, bomb.y + bomb.height };
        RECT ballRect = getBallRect();

        if (IntersectRect(&intersection, &ballRect, &bombRect)) 
        {
            MessageBox(gWnd, L"��ź�� �¾ҽ��ϴ�!", L"���", MB_OK);
            initBall();
            break;
        }
    }
}

// �� ��ü ��Ʈ�� �޾ƿ��� �Լ�
RECT Game::getBallRect() const 
{
    return RECT{ x - ballRadius, y - ballRadius, x + ballRadius, y + ballRadius };
}

// ���� �÷���, ����, ���� ��Ҵ��� Ȯ���ϴ� �Լ�
void Game::checkCollision()
{
    // �÷��� �浹 ����
    for (const auto& platform : platforms[currentRound])
    {
        jumpSpeed = -12;
        RECT platformRect = { platform.x, platform.y, platform.x + platform.width, platform.y + platform.height };
        RECT ballRect = getBallRect();

        if (IntersectRect(&intersection, &ballRect, &platformRect))
        {
            int ballCenter = ballRect.top + ballRadius;
            int platformCenter = platformRect.top + (platformRect.bottom - platformRect.top) / 2;
            int bP = ballCenter - platformCenter;

            if (bP > 0)
            {
                y = platformRect.bottom + ballRadius;
                speedY = -jumpSpeed;
                onGround = false;
            }
            else
            {
                y = platformRect.top - ballRadius;
                speedY = jumpSpeed;
                onGround = true;
            }
        }
    }
    // ���� �÷��� �浹 ����
    for (const auto& platform : trap[currentRound])
    {
        RECT trapRect = { platform.x, platform.y, platform.x + platform.width, platform.y + platform.height };
        RECT ballRect = getBallRect();

        if (IntersectRect(&intersection, &ballRect, &trapRect))
        {
            jumpSpeed = -20;
            y = trapRect.top - ballRadius;
            speedY = jumpSpeed;
            onGround = true;
        }
    }
    // �� ȹ�� ����
    for (size_t i = 0; i < star[currentRound].size(); ++i)
    {
        RECT starRect = { star[currentRound][i].x, star[currentRound][i].y,
                          star[currentRound][i].x + star[currentRound][i].width,
                          star[currentRound][i].y + star[currentRound][i].height };
        RECT ballRect = getBallRect();

        if (IntersectRect(&intersection, &ballRect, &starRect))
        {
            star[currentRound].erase(star[currentRound].begin() + i);
            --i;
            if (star[currentRound].empty())
            {
                if (currentRound + 1 >= platforms.size())
                {
                    MessageBox(gWnd, L"��� ���������� Ŭ�����߽��ϴ�!", L"���� ����", MB_OK);
                    isThread = false;
                    PostMessage(gWnd, WM_DESTROY, 0, 0);
                    return;
                }
                MessageBox(gWnd, L"��� ���� �����߽��ϴ�! ���� ����� �̵��մϴ�.", L"���� ����", MB_OK);

                currentRound = (currentRound + 1) % platforms.size();
                starCollected = std::vector<bool>(star[currentRound].size(), false);
                x = 150;
                y = 100;
                speedY = 0;
                onGround = true;
                break;
            }
        }
    }
}

// �� ��ǥ �������� �̵�
void Game::moveLeft() 
{
    x -= speedX;
}

// �� ��ǥ ���������� �̵�
void Game::moveRight() 
{
    x += speedX;
}