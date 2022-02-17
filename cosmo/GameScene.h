#include "Gamma.h"

// @todo bring into core; add a method on the
// struct for computing spherical coordinates
struct TrackingCamera {
  float azimuth = 0.0f;
  float altitude = 0.0f;
  float radius = 100.0f;
};

struct Player {
  Gamma::Vec3f position;
  Gamma::Vec3f momentum;
};

class GameScene : public Gamma::AbstractScene {
public:
  virtual void init() override;
  virtual void destroy() override;
  virtual void update(float dt) override;

private:
  void addCubeLattice();
  void updateCamera();

  TrackingCamera t_camera;
  Player player;
};