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

  input.on<MouseMoveEvent>("mousemove", [&](const MouseMoveEvent& event) {
    if (SDL_GetRelativeMouseMode()) {
      // @todo bring into core
      constexpr float ALTITUDE_LIMIT = Gm_HALF_PI * 0.999f;

      t_camera.altitude += event.deltaY / 1000.0f;
      t_camera.altitude = Gm_Clampf(t_camera.altitude, -ALTITUDE_LIMIT, ALTITUDE_LIMIT);
      t_camera.azimuth -= event.deltaX / 1000.0f;
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
  using namespace Gamma;

  float speed = 100.0f * dt;

  // @todo use drone orientation
  if (input.isKeyHeld(Key::W)) {
    p_drone.momentum += camera.orientation.getDirection() * speed;
  } else if (input.isKeyHeld(Key::S)) {
    p_drone.momentum -= camera.orientation.getDirection() * speed;
  }

  // @todo use drone orientation
  if (input.isKeyHeld(Key::A)) {
    p_drone.momentum += camera.orientation.getLeftDirection() * speed;
  } else if (input.isKeyHeld(Key::D)) {
    p_drone.momentum += camera.orientation.getRightDirection() * speed;
  }

  p_drone.position += p_drone.momentum * dt;

  // @todo bring into core
  Vec3f t_cameraPos = Vec3f(
    cosf(t_camera.altitude) * cosf(t_camera.azimuth) * t_camera.radius,
    sinf(t_camera.altitude) * t_camera.radius,
    cosf(t_camera.altitude) * sinf(t_camera.azimuth) * t_camera.radius
  );

  // @todo support mesh(...).objects[index] when
  // object indices are not expected to change
  for (auto& drone : mesh("drone").objects) {
    drone.position = p_drone.position;
    camera.position = drone.position + t_cameraPos;

    lookAt(drone);

    p_drone.targetRotation.x = -camera.orientation.pitch;
    p_drone.targetRotation.y = -camera.orientation.yaw;

    drone.rotation.x = Gm_Lerpf(drone.rotation.x, p_drone.targetRotation.x, dt);
    drone.rotation.y = Gm_LerpCircularf(drone.rotation.y, p_drone.targetRotation.y, dt, Gm_PI);

    commit(drone);
  }
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