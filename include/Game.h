#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <sstream>
#include <thread>

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>

#define WINDOW_W 800
#define WINDOW_H 600
#define MENU_H 40

#define FONT "res/font/monofonto.ttf"

enum Buttons {
  BtnRun, BtnDraw, BtnSpeed, BtnSize,
  BtnCount
};

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

  void drawMenu();

  void drawRunButton();

  void drawDrawButton();

  void drawSpeedButton();

  void drawSizeButton();

  static const short neighbors[8][2];

  int countNeighbors(int y, int x);

  void writeText(const char *text, int x, int y, TTF_Font *font, SDL_Color color);

  void handleClick(SDL_MouseButtonEvent *event);

  static bool insideRect(SDL_Rect rect, int x, int y);

  bool isDrawing();

  int getDelayTime();

  void addGlider();

private:
  static Game *instance;

  bool running{};
  SDL_Window *window{};
  SDL_Renderer *renderer{};

  float speed = 8;
  Uint32 frameStart{}, frameTime{};

  int size;
  int cols;
  int rows;
  bool **cells;

  TTF_Font *font{};

  bool drawing{};

  int mouseX{};
  int mouseY{};

  SDL_Rect btnRects[BtnCount]{};

  SDL_Color colorBlack = {0, 0, 0, 0};
};

#endif
