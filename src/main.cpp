#include "Game.h"

auto main() -> int {
  auto game = Game::Instance();
  game->init("Game of Life");

  while (game->isRunning()) {
    game->setFrameStart();
    game->handleEvents();

    if(!game->isDrawing()) { game->update(); }

    game->render();
    game->delay();
  }

  game->clean();

  return(EXIT_SUCCESS);
}
