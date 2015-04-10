#ifndef MATERIAL_H
#define MATERIAL_H

#include "IMaterial.h"

namespace zge
{

class Material : public IMaterial
{
private:
  unsigned _textureId;
  unsigned _shininess;

public:
  virtual ~Material() NOEXCEPT = default;

  void setTextureId(const unsigned);
  const unsigned getTextureId() const;
  void setShininess(const unsigned);
  const unsigned getShininess() const;
};

} // namespace zge

#endif