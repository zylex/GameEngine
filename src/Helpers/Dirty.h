#ifndef DIRTY_H
#define DIRTY_H

#include "Preprocessors.h"

namespace zge
{

class Dirty
{
private:
  bool flag;

public:
  // constructor
  Dirty();
  // destructor
  virtual ~Dirty() NOEXCEPT;
  // copy constructor
  Dirty(const Dirty&);
  // assignment operator
  Dirty& operator=(const Dirty&);
  // move constructor (C++11)
  Dirty(Dirty&&) NOEXCEPT;
  // move assignment operator (C++11)
  Dirty& operator=(Dirty&&) NOEXCEPT;

  virtual void clean();
  void dirty();
  const bool isDirty() const;
};

} // namespace zge

#endif
