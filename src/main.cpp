#include "Game.h"

auto main() -> int {
  auto game = new Game("Game of Life");

  while (game->isRunning()) {
    game->setFrameStart();
    game->handleEvents();
    game->render();

    if(!game->isDrawing()) { game->update(); }

    game->delay();
  }

  game->clean();

  return(EXIT_SUCCESS);
}
