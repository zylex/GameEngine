#include "Material.h"

namespace zge
{

void Material::setTextureId(const unsigned textureId)
{
  _textureId = textureId;
}

const unsigned Material::getTextureId() const { return _textureId; }

void Material::setShininess(const unsigned shininess)
{
  _shininess = shininess;
}

const unsigned Material::getShininess() const { return _shininess; }

} // namespace zge