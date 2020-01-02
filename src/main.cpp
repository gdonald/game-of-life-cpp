#include "Game.h"

auto main() -> int {
  Game::Instance()->init("Game of Life");

  while (Game::Instance()->isRunning()) {
    Game::Instance()->setFrameStart();
    Game::Instance()->handleEvents();
    Game::Instance()->update();
    Game::Instance()->render();
    Game::Instance()->delay();
  }

  Game::Instance()->clean();

  return(EXIT_SUCCESS);
}
