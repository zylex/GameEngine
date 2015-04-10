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

  void setTextureId(unsigned);
  unsigned getTextureId();
  void setShininess(unsigned);
  unsigned getShininess();
};

} // namespace zge

#endif