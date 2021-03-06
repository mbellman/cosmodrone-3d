#pragma once

#include "opengl/OpenGLRenderer.h"

namespace Gamma {
  void Gm_InitRendererResources(RendererBuffers& buffers, RendererShaders& shaders, const Area<uint32>& internalResolution);
  void Gm_DestroyRendererResources(RendererBuffers& buffers, RendererShaders& shaders);
}