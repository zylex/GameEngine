#ifndef MODELFACTORY_H
#define MODELFACTORY_H
#include "Preprocessors.h"

namespace zge
{
// TODO: Implement ModelFactory header
class ModelFactory
{
public:
  // constructor
  ModelFactory();
  // destructor
  virtual ~ModelFactory() NOEXCEPT;
  // copy constructor
  ModelFactory(const ModelFactory&);
  // assignment operator
  ModelFactory& operator=(const ModelFactory&);
  // move constructor (C++11)
  ModelFactory(ModelFactory&&) NOEXCEPT;
  // move assignment operator (C++11)
  ModelFactory& operator=(ModelFactory&&) NOEXCEPT;
};

} // namespace zge

#endif
