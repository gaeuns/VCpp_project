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

    int ballRadius = 15;
    int x = 150;
    int y = 100;
    int currentRound = 0;
    int speedY = 0;
    int speedX = 3;
    int gravity = 1;
    int jumpSpeed = -12;
    bool isThread = false;
    bool onGround = false;
    RECT intersection;

    std::thread ballThread;
    std::vector<bool>starCollected;

public:
    int gameMode;
    void createGame(HWND, HDC, int, int, int);
    void initBall();

    void startGame(HWND);
    void suspendGame();
    void resumeGame();
    void stopGame();
    
    void ballFunction();
    void moveBomb();
    void checkCollisionBomb();
    void checkCollision();
    void moveLeft();
    void moveRight();

    RECT getBallRect() const;
};
