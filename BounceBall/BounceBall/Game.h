#pragma once
#include <windows.h>
#include <thread>
#include <vector>

class Game
{
private:
    HWND gWnd;
    HDC ghdc;
    int ballRadius = 10;
    int x = 100;
    int y = 100;
    int currentRound = 0;
    int speedY = 0;
    int speedX = 3;
    int gravity = 1;
    int jumpSpeed = -15;
    bool isThread = false;
    bool onGround = false;
    std::thread ballThread;

public:
    void createGame(HWND, HDC);
    void startGame(HWND hWnd);
    void stopGame();
    void BallFunction();
    void applyGravity();
    void updatePosition();
    void checkCollision();
    void moveLeft();
    void moveRight();
    RECT getRect() const;
};
