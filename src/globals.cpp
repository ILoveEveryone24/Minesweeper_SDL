#include "../include/globals.h"

const int MINE_SIZE = 40;

const int GRID_W = 10;
const int GRID_H = 15;

const int WIDTH = MINE_SIZE * GRID_W;
const int HEIGHT = MINE_SIZE * GRID_H;

bool gameLost = false;
bool gameWon = false;
bool gameStarted = false;

int FPS = 0;

int timer = 0;
int mineAmount = GRID_W * GRID_H / 5;
