#ifndef MATERIAL_H
#define MATERIAL_H

#include "IMaterial.h"

namespace zge
{

class Material : public IMaterial
{
private:
  unsigned textureId;
  unsigned shininess;
  glm::vec4 color;

public:
  virtual ~Material() NOEXCEPT = default;

  void setTextureId(const unsigned);
  const unsigned getTextureId() const;
  void setShininess(const unsigned);
  const unsigned getShininess() const;
  void setColor(const glm::vec4);
  const glm::vec4 getColor() const;
};

} // namespace zge

#endif
