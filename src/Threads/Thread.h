#ifndef THREAD_H
#define THREAD_H

#include <thread>

#include "Preprocessors.h"

namespace zge
{

class Thread
{
private:
  std::thread theThread;

public:
  virtual ~Thread() NOEXCEPT = default;

  virtual void run();
  virtual void join();

  virtual std::thread& getThread();

  void cb();

  virtual void callback() = 0;
};

} // namespace zge

#endif
