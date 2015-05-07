#ifndef AFFINITYTHREAD_H
#define AFFINITYTHREAD_H

#include "Thread.h"

namespace zge
{

class AffinityThread : public Thread
{
private:
  const unsigned core;

public:
  AffinityThread(const unsigned);
  virtual ~AffinityThread() NOEXCEPT = default;

  virtual void run();

private:
  virtual void setAffinity(const unsigned);
};

} // namespace zge

#endif
