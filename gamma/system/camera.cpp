#include <cmath>

#include "system/camera.h"

namespace Gamma {
  Camera* Camera::active = nullptr;

  Vec3f ThirdPersonCamera::calculatePosition() const {
    return Vec3f(
      cosf(altitude) * cosf(azimuth) * radius,
      sinf(altitude) * radius,
      cosf(altitude) * sinf(azimuth) * radius
    );
  }
}