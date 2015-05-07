#ifndef NETWORKINGTHREAD_H
#define NETWORKINGTHREAD_H

#include "AffinityThread.h"

namespace zge
{

class NetworkingThread : public AffinityThread
{
private:
  static const unsigned CORE_NUMBER = 2;

public:
  NetworkingThread();
  virtual ~NetworkingThread() NOEXCEPT;

  virtual void callback() = 0;
};

} // namespace zge

#endif
