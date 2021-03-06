#pragma once

#include "system/traits.h"
#include "system/type_aliases.h"

#include "SDL.h"
#include "SDL_ttf.h"

namespace Gamma {
  struct Mesh;
  struct Light;

  struct RenderStats {
    uint32 gpuMemoryTotal;
    uint32 gpuMemoryUsed;
    bool isVSynced;
  };

  class AbstractRenderer : public Initable, public Renderable, public Destroyable {
  public:
    AbstractRenderer(SDL_Window* sdl_window): sdl_window(sdl_window) {};
    virtual ~AbstractRenderer() {};

    virtual void createMesh(const Mesh* mesh) {};
    virtual void createShadowMap(const Light* light) {};
    virtual void destroyMesh(const Mesh* mesh) {};
    virtual void destroyShadowMap(const Light* light) {};

    virtual Area<uint32>& getInternalResolution() final {
      return internalResolution;
    }

    virtual const RenderStats& getRenderStats() = 0;
    virtual void present() {};
    virtual void renderText(TTF_Font* font, const char* message, uint32 x, uint32 y, const Vec3f& color = Vec3f(1.0f), const Vec4f& background = Vec4f(0.0f)) {};

  protected:
    SDL_Window* sdl_window = nullptr;
    Area<uint32> internalResolution = { 1920, 1080 };
    RenderStats stats;
  };
}