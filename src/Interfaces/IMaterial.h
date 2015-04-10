#ifndef IMATERIAL_H
#define IMATERIAL_H

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
};

} // namespace zge

#endif