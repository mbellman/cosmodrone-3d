#include "Gamma.h"
#include "GameScene.h"

void GameScene::init() {
  using namespace Gamma;

  addCubeLattice();

  thirdPersonCamera.radius = 150.0f;

  auto& sun = createLight(LightType::DIRECTIONAL);

  sun.color = Vec3f(1.0f, 0.8f, 0.2f);
  sun.direction = Vec3f(0.5f, -1.0f, 1.0f);

  addMesh("drone", 1, Mesh::Model("./cosmo/obj/drone.obj"));

  mesh("drone").type = MeshType::REFLECTIVE;

  auto& drone = createObjectFrom("drone");

  drone.scale = 25.0f;
  drone.position = Vec3f(50.0f, 0, 0);

  commit(drone);

  input.on<MouseMoveEvent>("mousemove", [&](const MouseMoveEvent& event) {
    if (SDL_GetRelativeMouseMode()) {
      constexpr float ALTITUDE_LIMIT = Gm_HALF_PI * 0.999f;

      thirdPersonCamera.altitude += event.deltaY / 1000.0f;
      thirdPersonCamera.altitude = Gm_Clampf(thirdPersonCamera.altitude, -ALTITUDE_LIMIT, ALTITUDE_LIMIT);
      thirdPersonCamera.azimuth -= event.deltaX / 1000.0f;
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

  float droneMovementSpeed = 100.0f * dt;
  float droneRotationSpeed = 2.0f * dt;

  // @todo use drone orientation
  if (input.isKeyHeld(Key::W)) {
    playerDrone.momentum += camera.orientation.getDirection() * droneMovementSpeed;
  } else if (input.isKeyHeld(Key::S)) {
    playerDrone.momentum -= camera.orientation.getDirection() * droneMovementSpeed;
  }

  // @todo use drone orientation
  if (input.isKeyHeld(Key::A)) {
    playerDrone.momentum += camera.orientation.getLeftDirection() * droneMovementSpeed;
  } else if (input.isKeyHeld(Key::D)) {
    playerDrone.momentum += camera.orientation.getRightDirection() * droneMovementSpeed;
  }

  playerDrone.position += playerDrone.momentum * dt;

  // @todo support mesh(...).objects[index] when
  // object indices are not expected to change
  for (auto& drone : mesh("drone").objects) {
    drone.position = playerDrone.position;
    camera.position = drone.position + thirdPersonCamera.calculatePosition();

    lookAt(drone);

    playerDrone.targetRotation.x = -camera.orientation.pitch;
    playerDrone.targetRotation.y = -camera.orientation.yaw;

    drone.rotation.x = Gm_Lerpf(drone.rotation.x, playerDrone.targetRotation.x, droneRotationSpeed);
    drone.rotation.y = Gm_LerpCircularf(drone.rotation.y, playerDrone.targetRotation.y, droneRotationSpeed, Gm_PI);

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