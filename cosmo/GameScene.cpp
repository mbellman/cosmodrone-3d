#include "Gamma.h"
#include "GameScene.h"

void GameScene::init() {
  using namespace Gamma;

  addSpaceElevatorCable();

  addMesh("earth", 1, Mesh::Model("./cosmo/obj/ball.obj"));
  mesh("earth").texture = "./cosmo/images/earth.png";

  auto& earth = createObjectFrom("earth");

  earth.scale = 1000.0f;
  earth.rotation.x = Gm_HALF_PI;
  earth.rotation.z = -0.3f;

  commit(earth);

  thirdPersonCamera.radius = 75.0f;

  auto& sun = createLight(LightType::DIRECTIONAL);

  sun.color = Vec3f(1.0f, 0.9f, 0.5f);
  sun.direction = Vec3f(0.5f, -1.0f, 1.0f);

  addMesh("drone", 1, Mesh::Model("./cosmo/obj/drone.obj"));

  mesh("drone").type = MeshType::REFLECTIVE;

  auto& drone = createObjectFrom("drone");

  drone.scale = 10.0f;
  playerDrone.position = Vec3f(300.0f, 0, 0);

  commit(drone);

  input.on<MouseMoveEvent>("mousemove", [&](const MouseMoveEvent& event) {
    if (SDL_GetRelativeMouseMode()) {
      thirdPersonCamera.altitude += event.deltaY / 1000.0f;

      if (thirdPersonCamera.isUpsideDown()) {
        thirdPersonCamera.azimuth += event.deltaX / 1000.0f;
      } else {
        thirdPersonCamera.azimuth -= event.deltaX / 1000.0f;
      }
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

  float droneMovementSpeed = 30.0f * dt;
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

    lookAt(drone, thirdPersonCamera.isUpsideDown());

    playerDrone.targetRotation.x = camera.orientation.pitch;
    playerDrone.targetRotation.y = camera.orientation.yaw;

    drone.rotation.x = Gm_LerpCircularf(drone.rotation.x, playerDrone.targetRotation.x, droneRotationSpeed, Gm_PI);
    drone.rotation.y = Gm_LerpCircularf(drone.rotation.y, playerDrone.targetRotation.y, droneRotationSpeed, Gm_PI);

    commit(drone);
  }

  for (auto& earth : mesh("earth").objects) {
    earth.position.y = playerDrone.position.y - 9500.0f;
    earth.position.x = playerDrone.position.x;
    earth.position.z = playerDrone.position.z;

    commit(earth);
  }

  updateSpaceElevatorCable();
}

void GameScene::addSpaceElevatorCable() {
  using namespace Gamma;

  addMesh("elevator", 1, Mesh::Model("./cosmo/obj/elevator-cable.obj"));

  auto& elevator = createObjectFrom("elevator");

  mesh("elevator").texture = "./cosmo/images/carbon-fiber.png";
  mesh("elevator").normalMap = "./cosmo/images/carbon-fiber-normals.png";

  elevator.color = pVec4(100, 100, 120);

  commit(elevator);
}

void GameScene::updateSpaceElevatorCable() {
  using namespace Gamma;

  mesh("elevator").transformGeometry([&](const Vertex& baseVertex, Vertex& transformedVertex) {
    float heightRatio = Gm_Absf(baseVertex.position.y) / 1.0f;

    // Start from the base vertex position
    transformedVertex.position = baseVertex.position;

    // Apply scale to transformed geometry directly
    transformedVertex.position.x *= 100.0f;
    transformedVertex.position.y *= 5000.0f;
    transformedVertex.position.z *= 100.0f;

    // Curve toward the player drone near the vanishing points
    transformedVertex.position.x = Gm_Lerpf(transformedVertex.position.x, playerDrone.position.x, heightRatio);
    transformedVertex.position.z = Gm_Lerpf(transformedVertex.position.z, playerDrone.position.z, heightRatio);

    // Scroll UVs with drone y position, since the cable object
    // stays locked with the drone along the y axis
    transformedVertex.uv.y = baseVertex.position.y * 20.0f + playerDrone.position.y * 0.002f;
  });

  for (auto& elevator : mesh("elevator").objects) {
    elevator.position.y = playerDrone.position.y;

    commit(elevator);
  }
}