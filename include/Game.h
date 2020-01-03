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
  static Game *Instance();

  Game() = default;

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

  void drawWrapButton();

  void drawClearButton();

  void drawRandButton();

  void drawBtn(Buttons button, const char *title, SDL_Color color, int xLoc);

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

  int size{};
  int cols{};
  int rows{};
  bool **cells{};

  TTF_Font *font{};

  bool drawing{};
  bool wrap{};

  int mouseX{};
  int mouseY{};

  SDL_Rect btnRects[BtnCount]{};

  SDL_Color colorBlack = {0, 0, 0, 0};
  SDL_Color colorGrey = {127, 127, 127, 0};

  unsigned seed{};
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution;
};

#endif
