#include "Dirty.h"

namespace zge
{

bool Dirty::isDirty() { return this->isDirtyData; }

void Dirty::dirty() { this->isDirtyData = true; }

void Dirty::clean() { this->isDirtyData = false; }

} // namespace zge
