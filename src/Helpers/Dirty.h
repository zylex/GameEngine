#ifndef DIRTY_H
#define DIRTY_H

#include "Preprocessors.h"

namespace zge
{

class Dirty
{
private:
  bool isDirtyData;

public:
  virtual ~Dirty() NOEXCEPT = default;
  bool isDirty();
  void dirty();
  void clean();
};

} // namespace zge

#endif
