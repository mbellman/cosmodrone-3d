#include "Gamma.h"

// @todo bring into core; add a method on the
// struct for computing spherical coordinates
struct TrackingCamera {
  float azimuth = 0.0f;
  float altitude = 0.0f;
  float radius = 150.0f;
};

struct PlayerDrone {
  Gamma::Vec3f position;
  Gamma::Vec3f momentum;
  Gamma::Vec3f targetRotation;
};

class GameScene : public Gamma::AbstractScene {
public:
  virtual void init() override;
  virtual void destroy() override;
  virtual void update(float dt) override;

private:
  void addCubeLattice();

  TrackingCamera t_camera;
  PlayerDrone p_drone;
};