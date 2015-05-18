#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <vector>

#include "Preprocessors.h"

#include "Thread.h"

namespace zge
{

class ThreadManager
{
private:
  std::vector<Thread*> threads;

public:
  static ThreadManager& getInstance();
  // destructor
  virtual ~ThreadManager() NOEXCEPT;
  // copy constructor
  ThreadManager(const ThreadManager&);
  // assignment operator
  ThreadManager& operator=(const ThreadManager&);
  // move constructor (C++11)
  ThreadManager(ThreadManager&&) NOEXCEPT;
  // move assignment operator (C++11)
  ThreadManager& operator=(ThreadManager&&) NOEXCEPT;

  void startAll();
  void joinAll();
  void addThread(Thread*);

private:
  // constructor
  ThreadManager();
};

} // namespace zge

#endif
