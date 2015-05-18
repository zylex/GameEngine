#include "Material.h"

namespace zge
{

void Material::setTextureId(const unsigned textureId)
{
  this->textureId = textureId;
}

const unsigned Material::getTextureId() const { return this->textureId; }

void Material::setShininess(const unsigned shininess)
{
  this->shininess = shininess;
}

const unsigned Material::getShininess() const { return this->shininess; }

void Material::setColor(const glm::vec4 color) { this->color = color; }

const glm::vec4 Material::getColor() const { return this->color; }

} // namespace zge
