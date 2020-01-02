#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>

#define FPS 8
#define WINDOW_W 800
#define WINDOW_H 600
#define SIZE 10
#define COLS (WINDOW_W / SIZE)
#define ROWS (WINDOW_H / SIZE)

class Game {
public:
  static Game *Instance();

  Game();

  ~Game() = default;

  void update();

  void clean();

  void init(const char *title);

  void render();

  void handleEvents();

  bool isRunning();

  void delay();

  void setFrameStart();

  void drawGrid();

  void drawCells();

  static const short neighbors[8][2];

  int countNeighbors(int y, int x);

private:
  static Game *instance;

  bool running{};
  SDL_Window *window{};
  SDL_Renderer *renderer{};

  Uint32 frameStart{}, frameTime{};
  int delayTime = (int)(1000.0f / FPS);

  bool cells[ROWS][COLS]{};
};

#endif
