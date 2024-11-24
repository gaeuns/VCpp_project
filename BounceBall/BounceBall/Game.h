#pragma once
#include <windows.h>
#include <thread>
#include <vector>

class Game
{
private:
    HWND gWnd;
    HDC ghdc;
    HPEN myPen, osPen;
    HBRUSH myBrush, osBrush;
    HANDLE threadHandle;

    int ballRadius = 10;
    int x = 100;
    int y = 100;
    int currentRound = 0;
    int speedY = 0;
    int speedX = 3;
    int gravity = 1;
    int jumpSpeed = -12;
    bool isThread = false;
    bool isJoinble = true;
    bool onGround = false;

    std::thread ballThread;
    std::vector<bool>starCollected;

public:
    void createGame(HWND, HDC);
    void initBall();

    void startGame(HWND);
    void suspendGame();
    void resumeGame();
    void stopGame();
    
    void BallFunction();
    void setGravity();
    void updatePosition();
    void checkCollision();
    void moveLeft();
    void moveRight();

    RECT getBallRect() const;
};
