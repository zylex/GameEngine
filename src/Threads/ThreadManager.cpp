#include "ThreadManager.h"

namespace zge
{

ThreadManager& ThreadManager::getInstance()
{
  static ThreadManager instance;
  return instance;
}

ThreadManager::ThreadManager()
{
  // constructor
}

ThreadManager::~ThreadManager() NOEXCEPT
{
  // destructor
}

ThreadManager::ThreadManager(const ThreadManager& other)
{
  // copy constructor
}

ThreadManager& ThreadManager::operator=(const ThreadManager& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

ThreadManager::ThreadManager(ThreadManager&& other) NOEXCEPT
{
  // move constructor (C++11)
}

ThreadManager& ThreadManager::operator=(ThreadManager&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new ThreadManager(other));
}

void ThreadManager::startAll()
{
  // TODO: implement startAll
  // create networking thread

  // create simulation thread
}

void ThreadManager::joinAll()
{
  for (std::vector<Thread*>::iterator i = this->threads.begin();
       i IS_NOT this->threads.end(); ++i)
  {
    if ((*i)IS_NOT nullptr)
    {
      (*i)->join();
      delete (*i);
    }
  }
}

void ThreadManager::addThread(Thread* thrd) { this->threads.push_back(thrd); }

} // namespace zge
