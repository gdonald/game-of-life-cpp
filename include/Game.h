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
#define MENU_H 40

#define FONT "res/font/monofonto.ttf"

enum Buttons {
  BtnRun, BtnDraw,
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

  static const short neighbors[8][2];

  int countNeighbors(int y, int x);

  void writeText(const char *text, int x, int y, TTF_Font *font, SDL_Color color);

  void handleClick(SDL_MouseButtonEvent *event);

  static bool insideRect(SDL_Rect rect, int x, int y);

  bool isDrawing();

private:
  static Game *instance;

  bool running{};
  SDL_Window *window{};
  SDL_Renderer *renderer{};

  Uint32 frameStart{}, frameTime{};
  int delayTime = (int)(1000.0f / FPS);

  bool cells[ROWS][COLS]{};

  TTF_Font *font;

  bool drawing{};

  int mouseX{};
  int mouseY{};

  SDL_Rect btnRects[BtnCount]{};
};

#endif
