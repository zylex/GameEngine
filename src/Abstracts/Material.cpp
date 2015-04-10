#include "Material.h"

namespace zge
{

void Material::setTextureId(unsigned textureId) { _textureId = textureId; }

unsigned Material::getTextureId() { return _textureId; }

void Material::setShininess(unsigned shininess) { _shininess = shininess; }

unsigned Material::getShininess() { return _shininess; }

} // namespace zge