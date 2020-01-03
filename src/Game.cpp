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
  size = 10;
  cols = WINDOW_W / size;
  rows = WINDOW_H / size;

  cells = new bool*[rows];
  for(int y = 0; y < rows; y++) {
    cells[y] = new bool[cols];
    for(int x = 0; x < cols; x++) {
      cells[y][x] = false;
    }
  }

  addGlider();
}

void Game::update() {
  bool newCells[rows][cols];

  for (auto & newCell : newCells)
    for (bool & x : newCell)
      x = false;

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {

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

  for (int y = 0; y < rows; y++)
    for (int x = 0; x < cols; x++)
      cells[y][x] = newCells[y][x];
}

int Game::countNeighbors(int y, int x) {
  int count = 0;

  for (auto n : neighbors) {
    int yy = y + n[0];
    int xx = x + n[1];

    if (wrap) {
      if (yy <= -1) yy = rows - 1;
      if (yy >= rows) yy = 0;
      if (xx <= -1) xx = rows - 1;
      if (xx >= cols) xx = 0;
    } else {
      if (yy < 0 || xx < 0 || yy >= rows || xx >= cols) { continue; }
    }

    if (yy < 0 || xx < 0 || yy >= rows || xx >= cols) { continue; }

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

  int window_h = WINDOW_H + MENU_H;

  window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, window_h, 0);

  if (!window) {
    printf("Could not create window: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_SetWindowMinimumSize(window, WINDOW_W, window_h);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (!renderer) {
    printf("Count not get renderer! SDL Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  if (TTF_Init() == -1) {
    printf("TTF_Init failed: %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }

  font = TTF_OpenFont(FONT, 21);
  if (font == nullptr) {
    printf("Failed to load font! Error: %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }

  drawing = false;
  running = true;
}

void Game::render() {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  drawGrid();
  drawCells();
  drawMenu();

  SDL_RenderPresent(renderer);
}

void Game::drawGrid() {
  SDL_SetRenderDrawColor(renderer, 0xee, 0xee, 0xee, 0xff);

  for (int x = 0; x <= WINDOW_W; x += size)
    SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_H);

  for (int y = 0; y <= WINDOW_H; y += size)
    SDL_RenderDrawLine(renderer, 0, y, WINDOW_W, y);
}

void Game::drawCells() {
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      if (cells[y][x]) {
        Sint16 x0 = x * size;
        Sint16 x1 = (x + 1) * size;
        Sint16 y0 = y * size;
        Sint16 y1 = (y + 1) * size;

        const Sint16 vx[4] = {x0, x1, x1, x0};
        const Sint16 vy[4] = {y0, y0, y1, y1};
        filledPolygonRGBA(renderer, vx, vy, 4, 0x00, 0x00, 0x00, 0xff);
      }
    }
  }
}

void Game::drawMenu() {
  Sint16 x0 = 0;
  Sint16 x1 = WINDOW_W;
  Sint16 x2 = WINDOW_W;
  Sint16 x3 = 0;

  Sint16 y0 = WINDOW_H;
  Sint16 y1 = WINDOW_H;
  Sint16 y2 = WINDOW_H + MENU_H;
  Sint16 y3 = WINDOW_H + MENU_H;

  const Sint16 vx[4] = {x0, x1, x2, x3};
  const Sint16 vy[4] = {y0, y1, y2, y3};

  filledPolygonRGBA(renderer, vx, vy, 4, 0xaa, 0xaa, 0xaa, 0xff);

  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xff, 0xff);
  SDL_RenderDrawLine(renderer, 0, WINDOW_H, WINDOW_W, WINDOW_H);

  drawing ? drawRunButton() : drawDrawButton();
  drawSpeedButton();
  drawSizeButton();
  drawWrapButton();
}

void Game::drawRunButton() {
  btnRects[BtnRun].x = 9;
  btnRects[BtnRun].y = WINDOW_H + 8;
  btnRects[BtnRun].w = 62;
  btnRects[BtnRun].h = 32;

  Sint16 x0 = btnRects[BtnRun].x;
  Sint16 x1 = btnRects[BtnRun].x + btnRects[BtnRun].w;
  Sint16 x2 = btnRects[BtnRun].x + btnRects[BtnRun].w;
  Sint16 x3 = btnRects[BtnRun].x;

  Sint16 y0 = btnRects[BtnRun].y;
  Sint16 y1 = btnRects[BtnRun].y;
  Sint16 y2 = WINDOW_H + btnRects[BtnRun].h;
  Sint16 y3 = WINDOW_H + btnRects[BtnRun].h;

  const Sint16 vx[4] = {x0, x1, x2, x3};
  const Sint16 vy[4] = {y0, y1, y2, y3};

  filledPolygonRGBA(renderer, vx, vy, 4, 0xaa, 0xaa, 0xff, 0xff);
  writeText("RUN", 24, WINDOW_H + 8, font, colorBlack);
}

void Game::drawDrawButton() {
  btnRects[BtnDraw].x = 9;
  btnRects[BtnDraw].y = WINDOW_H + 8;
  btnRects[BtnDraw].w = 62;
  btnRects[BtnDraw].h = 32;

  Sint16 x0 = btnRects[BtnDraw].x;
  Sint16 x1 = btnRects[BtnDraw].x + btnRects[BtnDraw].w;
  Sint16 x2 = btnRects[BtnDraw].x + btnRects[BtnDraw].w;
  Sint16 x3 = btnRects[BtnDraw].x;

  Sint16 y0 = btnRects[BtnDraw].y;
  Sint16 y1 = btnRects[BtnDraw].y;
  Sint16 y2 = WINDOW_H + btnRects[BtnDraw].h;
  Sint16 y3 = WINDOW_H + btnRects[BtnDraw].h;

  const Sint16 vx[4] = {x0, x1, x2, x3};
  const Sint16 vy[4] = {y0, y1, y2, y3};

  filledPolygonRGBA(renderer, vx, vy, 4, 0xaa, 0xaa, 0xff, 0xff);
  writeText("DRAW", 18, WINDOW_H + 8, font, colorBlack);
}

void Game::drawSpeedButton() {
  btnRects[BtnSpeed].x = 82;
  btnRects[BtnSpeed].y = WINDOW_H + 8;
  btnRects[BtnSpeed].w = 92;
  btnRects[BtnSpeed].h = 32;

  Sint16 x0 = btnRects[BtnSpeed].x;
  Sint16 x1 = btnRects[BtnSpeed].x + btnRects[BtnSpeed].w;
  Sint16 x2 = btnRects[BtnSpeed].x + btnRects[BtnSpeed].w;
  Sint16 x3 = btnRects[BtnSpeed].x;

  Sint16 y0 = btnRects[BtnSpeed].y;
  Sint16 y1 = btnRects[BtnSpeed].y;
  Sint16 y2 = WINDOW_H + btnRects[BtnSpeed].h;
  Sint16 y3 = WINDOW_H + btnRects[BtnSpeed].h;

  const Sint16 vx[4] = {x0, x1, x2, x3};
  const Sint16 vy[4] = {y0, y1, y2, y3};

  filledPolygonRGBA(renderer, vx, vy, 4, 0xaa, 0xaa, 0xff, 0xff);

  std::ostringstream label;
  label << "SPEED " << (int)(speed / 4);

  writeText(label.str().c_str(), 90, WINDOW_H + 8, font, colorBlack);
}

void Game::drawSizeButton() {
  btnRects[BtnSize].x = 185;
  btnRects[BtnSize].y = WINDOW_H + 8;
  btnRects[BtnSize].w = 82;
  btnRects[BtnSize].h = 32;

  Sint16 x0 = btnRects[BtnSize].x;
  Sint16 x1 = btnRects[BtnSize].x + btnRects[BtnSize].w;
  Sint16 x2 = btnRects[BtnSize].x + btnRects[BtnSize].w;
  Sint16 x3 = btnRects[BtnSize].x;

  Sint16 y0 = btnRects[BtnSize].y;
  Sint16 y1 = btnRects[BtnSize].y;
  Sint16 y2 = WINDOW_H + btnRects[BtnSize].h;
  Sint16 y3 = WINDOW_H + btnRects[BtnSize].h;

  const Sint16 vx[4] = {x0, x1, x2, x3};
  const Sint16 vy[4] = {y0, y1, y2, y3};

  filledPolygonRGBA(renderer, vx, vy, 4, 0xaa, 0xaa, 0xff, 0xff);

  std::ostringstream label;
  label << "SIZE " << (int)(size / 5);

  writeText(label.str().c_str(), 193, WINDOW_H + 8, font, colorBlack);
}

void Game::drawWrapButton() {
  btnRects[BtnWrap].x = 278;
  btnRects[BtnWrap].y = WINDOW_H + 8;
  btnRects[BtnWrap].w = 62;
  btnRects[BtnWrap].h = 32;

  Sint16 x0 = btnRects[BtnWrap].x;
  Sint16 x1 = btnRects[BtnWrap].x + btnRects[BtnWrap].w;
  Sint16 x2 = btnRects[BtnWrap].x + btnRects[BtnWrap].w;
  Sint16 x3 = btnRects[BtnWrap].x;

  Sint16 y0 = btnRects[BtnWrap].y;
  Sint16 y1 = btnRects[BtnWrap].y;
  Sint16 y2 = WINDOW_H + btnRects[BtnWrap].h;
  Sint16 y3 = WINDOW_H + btnRects[BtnWrap].h;

  const Sint16 vx[4] = {x0, x1, x2, x3};
  const Sint16 vy[4] = {y0, y1, y2, y3};

  filledPolygonRGBA(renderer, vx, vy, 4, 0xaa, 0xaa, 0xff, 0xff);

  SDL_Color color = wrap ? colorBlack : colorGrey;
  writeText("WRAP", 288, WINDOW_H + 8, font, color);
}

void Game::handleEvents() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        running = false;
        break;
      case SDL_MOUSEBUTTONUP:
        SDL_GetMouseState(&mouseX, &mouseY);
        handleClick(&event.button);
        break;
    }
  }
}

void Game::handleClick(SDL_MouseButtonEvent *event) {
  if (drawing && mouseY < WINDOW_H) {
    int x = mouseX / size;
    int y = mouseY / size;
    cells[y][x] = !cells[y][x];
    return;
  }

  if(drawing && insideRect(btnRects[BtnRun], mouseX, mouseY)) {
    drawing = false;
    return;
  }

  if(!drawing && insideRect(btnRects[BtnDraw], mouseX, mouseY)) {
    drawing = true;
    return;
  }

  if(insideRect(btnRects[BtnSpeed], mouseX, mouseY)) {
    float newSpeed = speed + 4;
    if (newSpeed > 36) { newSpeed = 4; }
    speed = newSpeed;

    return;
  }

  if(insideRect(btnRects[BtnSize], mouseX, mouseY)) {
    int newSize = size + 5;
    if (newSize > 45) { newSize = 5; }
    size = newSize;

    cols = WINDOW_W / size;
    rows = WINDOW_H / size;

    delete [] cells;
    cells = new bool*[rows];

    for(int y = 0; y < rows; y++) {
      cells[y] = new bool[cols];
      for (int x = 0; x < cols; x++) {
        cells[y][x] = false;
      }
    }

    addGlider();
    return;
  }

  if(!wrap && insideRect(btnRects[BtnWrap], mouseX, mouseY)) {
    wrap = true;
    return;
  }

  if(wrap && insideRect(btnRects[BtnWrap], mouseX, mouseY)) {
    wrap = false;
    return;
  }
}

bool Game::insideRect(SDL_Rect rect, int x, int y) {
  return x > rect.x &&
         x < rect.x + rect.w &&
         y > rect.y &&
         y < rect.y + rect.h;
}

bool Game::isRunning() {
  return running;
}

bool Game::isDrawing() {
  return drawing;
}

void Game::delay() {
  frameTime = SDL_GetTicks() - frameStart;
  if (frameTime < getDelayTime()) { SDL_Delay((int) (getDelayTime() - frameTime)); }
}

void Game::setFrameStart() {
  frameStart = SDL_GetTicks();
}

void Game::writeText(const char *text, int x, int y, TTF_Font *font, SDL_Color color) {
  int w, h;

  SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  TTF_SizeText(font, text, &w, &h);
  SDL_Rect rect = {.x = x, .y = y, .w = w, .h = h};

  SDL_RenderCopy(renderer, texture, nullptr, &rect);

  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);
}

int Game::getDelayTime() {
  return (int)(1000.0f / speed);
}

void Game::addGlider() {
  cells[0][2] = true;
  cells[1][2] = true;
  cells[2][2] = true;
  cells[2][1] = true;
  cells[1][0] = true;
}
