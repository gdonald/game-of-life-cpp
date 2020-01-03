#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <sstream>
#include <thread>

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <random>

#define WINDOW_W 800
#define WINDOW_H 600
#define MENU_H 40

#define FONT "res/font/monofonto.ttf"

enum Buttons {
  BtnRun, BtnDraw, BtnSpeed, BtnSize,
  BtnWrap, BtnClear, BtnRand,
  BtnCount
};

class Game {
public:
  explicit Game(const char *title);

  ~Game() = default;

  void update();

  void clean();

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

  void drawWrapButton();

  void drawClearButton();

  void drawRandButton();

  void drawBtn(Buttons button, const char *title, SDL_Color color, int xLoc);

  static const short neighbors[8][2];

  int countNeighbors(int y, int x);

  void writeText(const char *text, int x, int y, TTF_Font *font, SDL_Color color);

  void handleClick();

  static bool insideRect(SDL_Rect rect, int x, int y);

  bool isDrawing();

  Uint32 getDelayTime();

  void addGlider();

private:
  bool **cells{};

  float speed = 8;

  unsigned seed{};
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution;

  int mouseX{};
  int mouseY{};
  int size{};
  int cols{};
  int rows{};

  Uint32 frameStart{}, frameTime{};

  SDL_Rect btnRects[BtnCount]{};

  SDL_Color colorBlack = {0, 0, 0, 0};
  SDL_Color colorGrey = {127, 127, 127, 0};

  SDL_Window *window{};
  SDL_Renderer *renderer{};
  TTF_Font *font{};

  bool running{};
  bool drawing{};
  bool wrap{};
};

#endif
