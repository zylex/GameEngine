#ifndef IMATERIAL_H
#define IMATERIAL_H

#include <glm/vec4.hpp>

#include "Preprocessors.h"

namespace zge
{
// forward declarations
class IMaterial
{
public:
  virtual ~IMaterial() NOEXCEPT = default;

  virtual void setShininess(const unsigned) = 0;
  virtual const unsigned getShininess() const = 0;
  virtual void setTextureId(const unsigned) = 0;
  virtual const unsigned getTextureId() const = 0;
  virtual void setColor(const glm::vec4) = 0;
  virtual const glm::vec4 getColor() const = 0;
};

} // namespace zge

#endif
