#include "Gamma.h"

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
  void addEarth();
  void addSpaceElevatorCable();
  void addSpaceElevatorModules();
  void updateSpaceElevatorCable();
  void updateSpaceElevatorModules();

  Gamma::ThirdPersonCamera thirdPersonCamera;
  PlayerDrone playerDrone;
};