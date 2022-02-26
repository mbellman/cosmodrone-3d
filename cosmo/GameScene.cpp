#include "Gamma.h"
#include "GameScene.h"

using namespace Gamma;

void GameScene::init() {
  addEarth();
  addSpaceElevatorCable();
  addSpaceElevatorModules();

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
  updateSpaceElevatorModules();
}

void GameScene::addEarth() {
  addMesh("earth", 1, Mesh::Model("./cosmo/obj/ball.obj"));
  mesh("earth").texture = "./cosmo/images/earth.png";

  auto& earth = createObjectFrom("earth");

  earth.scale = 1000.0f;
  earth.rotation.x = Gm_HALF_PI;
  earth.rotation.z = -0.3f;

  commit(earth);
}

void GameScene::addSpaceElevatorCable() {
  addMesh("elevator", 1, Mesh::Model("./cosmo/obj/elevator-cable.obj"));

  auto& elevator = createObjectFrom("elevator");

  mesh("elevator").texture = "./cosmo/images/carbon-fiber.png";
  mesh("elevator").normalMap = "./cosmo/images/carbon-fiber-normals.png";

  elevator.color = pVec4(100, 100, 120);

  commit(elevator);
}

void GameScene::addSpaceElevatorModules() {
  addMesh("tube", 1, Mesh::Model("./cosmo/obj/elevator-tube.obj"));

  auto& tube = createObjectFrom("tube");

  tube.scale = 500.0f;
  tube.position.y = 500.0f;

  commit(tube);
}

void GameScene::updateSpaceElevatorCable() {
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
    transformedVertex.uv.y = baseVertex.position.y * 20.0f + playerDrone.position.y * 0.004f;
  });

  for (auto& elevator : mesh("elevator").objects) {
    elevator.position.y = playerDrone.position.y;

    commit(elevator);
  }
}

void GameScene::updateSpaceElevatorModules() {
  // @todo @cleanup this code works but is a bit messy/ad-hoc.
  // organize and refactor it a little and it'll be good to go
  auto limit = [](float x) {
    return 1.0f - 1.0f / (x + 1.0f);
  };

  float limitFactor = limit(Gm_Absf(playerDrone.position.y / 5000.0f));

  float modulePosition = 0.0f;
  float directionFactor = playerDrone.position.y > modulePosition ? -1.0f : 1.0f;
  float moduleYOffset = (0.0f + 5000.0f * limitFactor * directionFactor);

  for (auto& tube : mesh("tube").objects) {
    float modulePosition = 0.0f;  // @temp
    float directionFactor = playerDrone.position.y > modulePosition ? -1.0f : 1.0f;
    float moduleYOffset = modulePosition + 5000.0f * limitFactor * directionFactor;
    float heightRatio = Gm_Absf(moduleYOffset) / 5000.0f;

    tube.position.x = Gm_Lerpf(0, playerDrone.position.x, heightRatio);
    tube.position.z = Gm_Lerpf(0, playerDrone.position.z, heightRatio);
    tube.position.y = playerDrone.position.y + moduleYOffset;
    tube.scale = 500.0f * (1.0f - heightRatio);

    commit(tube);
  }
}