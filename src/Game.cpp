#include <thread>
#include "Game.h"

const short Game::neighbors[8][2] = {{-1, -1},
                                     {-1, 0},
                                     {-1, 1},
                                     {0,  1},
                                     {1,  1},
                                     {1,  0},
                                     {1,  -1},
                                     {0,  -1}};

Game *Game::instance = nullptr;

Game *Game::Instance() {
  if (!instance) {
    instance = new Game;
  }

  return instance;
}

Game::Game() {
  for (int y = 0; y < ROWS; y++)
    for (int x = 0; x < COLS; x++)
      cells[y][x] = false;

  cells[10][15] = true;
  cells[11][15] = true;
  cells[12][15] = true;
  cells[12][14] = true;
  cells[11][13] = true;
}

void Game::update() {
  bool newCells[ROWS][COLS];

  for (int y = 0; y < ROWS; y++)
    for (int x = 0; x < COLS; x++)
      newCells[y][x] = false;

  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {

      // https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
      // 1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
      // 2. Any live cell with two or three live neighbours lives on to the next generation.
      // 3. Any live cell with more than three live neighbours dies, as if by overpopulation.
      // 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

      int neighborCount = countNeighbors(y, x);

      if (cells[y][x]) {
        if (neighborCount < 2 || neighborCount > 3) {
          newCells[y][x] = false;
        } else if (neighborCount == 2 || neighborCount == 3) {
          newCells[y][x] = true;
        }
      } else if (neighborCount == 3) {
        newCells[y][x] = true;
      }
    }
  }

  for (int y = 0; y < ROWS; y++)
    for (int x = 0; x < COLS; x++)
      cells[y][x] = newCells[y][x];
}

int Game::countNeighbors(int y, int x) {
  int count = 0;

  for (auto n : neighbors) {
    int yy = y + n[0];
    int xx = x + n[1];

    if (yy < 0 || xx < 0 || yy >= ROWS || xx >= COLS) { continue; } // TODO: wrap around
    if (cells[yy][xx]) { count++; }
  }

  return count;
}

void Game::clean() {

}

void Game::init(const char *title) {
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, 0);

  if (!window) {
    printf("Could not create window: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_SetWindowMinimumSize(window, WINDOW_W, WINDOW_H);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (!renderer) {
    printf("Count not get renderer! SDL Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  running = true;
}

void Game::render() {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  drawGrid();
  drawCells();

  SDL_RenderPresent(renderer);
}

void Game::drawGrid() {
  SDL_SetRenderDrawColor(renderer, 0xdd, 0xdd, 0xdd, 0xff);

  for (int x = 0; x <= WINDOW_W; x += SIZE)
    SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_H);

  for (int y = 0; y <= WINDOW_H; y += SIZE)
    SDL_RenderDrawLine(renderer, 0, y, WINDOW_W, y);
}

void Game::drawCells() {
  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      if (cells[y][x]) {
        Sint16 x0 = x * SIZE;
        Sint16 x1 = (x + 1) * SIZE;
        Sint16 y0 = y * SIZE;
        Sint16 y1 = (y + 1) * SIZE;

        const Sint16 vx[4] = {x0, x1, x1, x0};
        const Sint16 vy[4] = {y0, y0, y1, y1};
        filledPolygonRGBA(renderer, vx, vy, 4, 0x00, 0x00, 0x00, 0xff);
      }
    }
  }
}

void Game::handleEvents() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        running = false;
        break;
    }
  }
}

bool Game::isRunning() {
  return running;
}

void Game::delay() {
  frameTime = SDL_GetTicks() - frameStart;
  if (frameTime < delayTime) { SDL_Delay((int) (delayTime - frameTime)); }
}

void Game::setFrameStart() {
  frameStart = SDL_GetTicks();
}
