#pragma once

#include <vector>

#include "math/plane.h"
#include "system/traits.h"
#include "system/type_aliases.h"

namespace Gamma {
  struct ColorAttachment {
    GLuint index;
    GLuint textureId;
    GLenum textureUnit;
  };

  enum ColorFormat {
    R,
    R16,
    RG,
    RG16,
    RGB,
    RGB16,
    RGBA,
    RGBA8,
    RGBA16
  };

  class OpenGLFrameBuffer : public Initable, public Destroyable {
  public:
    virtual void init() override;
    virtual void destroy() override;
    void addColorAttachment(ColorFormat format);
    void addColorAttachment(ColorFormat format, uint32 unit);
    void addColorAttachment(ColorFormat format, uint32 unit, GLint clamp);
    void addDepthAttachment();
    void addDepthStencilAttachment();
    void bindColorAttachments();
    void read(uint32 offset = 0);
    void setSize(const Area<uint32>& size);
    void shareDepthStencilAttachment(const OpenGLFrameBuffer& target);
    void write();
    void writeToAttachment(uint32 attachment);

  private:
    GLuint fbo = 0;
    GLuint depthTextureId = 0;
    GLuint depthStencilTextureId = 0;
    std::vector<ColorAttachment> colorAttachments;
    Area<uint32> size;
  };

  class OpenGLCubeMap : public Initable, public Destroyable {
  public:
    virtual void init() override;
    virtual void destroy() override;
    void read();
    void addColorAttachment(ColorFormat format, uint32 unit);
    void addDepthAttachment(uint32 unit);
    void bindColorAttachments();
    void setSize(const Area<uint32>& size);
    void write();
    void writeToFace(uint8 face);

  private:
    GLuint fbo = 0;
    GLenum depthTextureUnit;
    GLuint depthTextureId = 0;
    std::vector<ColorAttachment> colorAttachments;
    Area<uint32> size;
  };
}