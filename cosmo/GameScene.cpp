#include "Gamma.h"
#include "GameScene.h"

void GameScene::init() {
  using namespace Gamma;

  addCubeLattice();

  auto& sun = createLight(LightType::DIRECTIONAL);

  sun.color = Vec3f(1.0f, 0.8f, 0.2f);
  sun.direction = Vec3f(0.5f, -1.0f, 1.0f);

  addMesh("drone", 1, Mesh::Cube());

  auto& drone = createObjectFrom("drone");

  drone.scale = Vec3f(10.0f, 10.0f, 30.0f);
  drone.color = pVec4(200, 0, 200);
  drone.position = Vec3f(50.0f, 0, 0);

  commit(drone);

  // @todo bring into core
  auto updateCamera = [&]() {
    Vec3f pos = Vec3f(
      cosf(t_camera.altitude) * cosf(t_camera.azimuth) * t_camera.radius,
      sinf(t_camera.altitude) * t_camera.radius,
      cosf(t_camera.altitude) * sinf(t_camera.azimuth) * t_camera.radius
    );

    camera.position = drone.position + pos;

    lookAt(drone);
  };

  updateCamera();

  input.on<MouseMoveEvent>("mousemove", [&, updateCamera](const MouseMoveEvent& event) {
    if (SDL_GetRelativeMouseMode()) {
      // @todo bring into core
      t_camera.altitude += event.deltaY / 1000.0f;
      t_camera.azimuth -= event.deltaX / 1000.0f;

      updateCamera();
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

void GameScene::addCubeLattice() {
  using namespace Gamma;

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
}