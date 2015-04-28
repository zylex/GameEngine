#include "Dirty.h"

namespace zge
{

Dirty::Dirty() : flag(false)
{
  // constructor
}

Dirty::~Dirty() NOEXCEPT
{
  // destructor
}

Dirty::Dirty(const Dirty& other)
{
  // copy constructor
}

Dirty& Dirty::operator=(const Dirty& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

Dirty::Dirty(Dirty&& other) NOEXCEPT
{
  // move constructor (C++11)
}

Dirty& Dirty::operator=(Dirty&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new Dirty(other));
}

void Dirty::clean()
{
  // TODO
  this->flag = false;
}

void Dirty::dirty()
{
  // TODO
  this->flag = true;
}

const bool Dirty::isDirty() const
{
  // TODO
  return this->flag;
}

} // namespace zge
