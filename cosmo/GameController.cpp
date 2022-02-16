#include "GameController.h"
#include "GameScene.h"

void GameController::init() {
  enterScene(new GameScene());
}

void GameController::destroy() {
  // ...
}