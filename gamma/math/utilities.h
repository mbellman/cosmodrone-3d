#pragma once

namespace Gamma {
  constexpr float Gm_PI = 3.141592f;
  constexpr float Gm_TAU = Gm_PI * 2.0f;
  constexpr float Gm_HALF_PI = Gm_PI / 2.0f;

  inline float Gm_Clampf(float value, float min, float max) {
    return value > max ? max : value < min ? min : value;
  }
}