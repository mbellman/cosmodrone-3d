#include "Gamma.h"
#include "GameScene.h"

void GameScene::init() {
  using namespace Gamma;

  Gm_EnableFlags(Gamma::FREE_CAMERA_MODE);

  addMesh("plane", 1, Mesh::Plane(5));
  addMesh("cube", 1, Mesh::Cube());

  auto& plane = createObjectFrom("plane");
  auto& cube = createObjectFrom("cube");

  plane.scale = 1000.0f;

  cube.scale = 20.0f;
  cube.position.y = 20.0f;
  cube.color = pVec4(255, 50, 50);

  commit(plane);
  commit(cube);

  auto& light = createLight(LightType::SPOT_SHADOWCASTER);

  light.position = cube.position + Vec3f(-30.0f, 30.0f, -30.0f);
  light.direction = cube.position - light.position;
  light.color = Vec3f(1.0f, 0.9f, 0.2f);
  light.radius = 500.0f;

  camera.position = Vec3f(-100.0f, 75.0f, -150.0f);

  lookAt(cube);

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