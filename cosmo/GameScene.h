#include "Gamma.h"

class GameScene : public Gamma::AbstractScene {
public:
  virtual void init() override;
  virtual void destroy() override;
  virtual void update(float dt) override;

private:
  // ...
};