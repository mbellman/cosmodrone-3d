#include "Gamma.h"
#include "GameController.h"

int main(int argc, char* argv[]) {
  Gamma::Window window;

  window.setTitle("Cosmodrone");
  window.setRenderMode(Gamma::RenderMode::OPENGL);
  window.setController(new GameController());
  window.open();

  return 0;
}