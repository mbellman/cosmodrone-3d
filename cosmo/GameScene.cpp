#include "Gamma.h"
#include "GameScene.h"

void GameScene::init() {
  using namespace Gamma;

  Gm_EnableFlags(Gamma::FREE_CAMERA_MODE);

  addMesh("cube", 1000, Mesh::Cube());

  for (int x = 0; x < 10; x++) {
    for (int y = 0; y < 10; y++) {
      for (int z = 0; z < 10; z++) {
        auto& cube = createObjectFrom("cube");

        cube.scale = 10.0f;

        cube.position = Vec3f(
          (x - 5) * 200.0f,
          (y - 5) * 200.0f,
          (z - 5) * 200.0f
        );

        commit(cube);
      }
    }
  }

  input.on<MouseMoveEvent>("mousemove", [&](const MouseMoveEvent& event) {
    if (SDL_GetRelativeMouseMode()) {
      camera.orientation.pitch += event.deltaY / 1000.0f;
      camera.orientation.yaw += event.deltaX / 1000.0f;
    }
  });

  input.on<MouseButtonEvent>("mousedown", [&](const MouseButtonEvent& event) {
    if (!SDL_GetRelativeMouseMode()) {
      SDL_SetRelativeMouseMode(SDL_TRUE);
    }
  });

  input.on<Key>("keyup", [&](Key key) {
    if (key == Key::ESCAPE) {
      SDL_SetRelativeMouseMode(SDL_FALSE);
    }

    if (key == Key::V) {
      if (Gm_IsFlagEnabled(GammaFlags::VSYNC)) {
        Gm_DisableFlags(GammaFlags::VSYNC);
      } else {
        Gm_EnableFlags(GammaFlags::VSYNC);
      }
    }
  });
}

void GameScene::destroy() {
  // ...
}

void GameScene::update(float dt) {
  // ...
}