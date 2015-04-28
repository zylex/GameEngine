#include "ModelFactory.h"
// TODO: Implement ModelFactory
namespace zge
{

ModelFactory::ModelFactory()
{
  // constructor
}

ModelFactory::~ModelFactory() NOEXCEPT
{
  // destructor
}

ModelFactory::ModelFactory(const ModelFactory& other)
{
  // copy constructor
}

ModelFactory& ModelFactory::operator=(const ModelFactory& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

ModelFactory::ModelFactory(ModelFactory&& other) NOEXCEPT
{
  // move constructor (C++11)
}

ModelFactory& ModelFactory::operator=(ModelFactory&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new ModelFactory(other));
}

} // namespace zge
