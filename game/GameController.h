#include "Gamma.h"

class GameController : public Gamma::AbstractController {
public:
  virtual void init() override;
  virtual void destroy() override;
};