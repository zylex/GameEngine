#include <iostream>

#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) ||               \
                         (defined(__APPLE__) && defined(__MACH__)))
#include <pthread.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include "AffinityThread.h"

namespace zge
{

AffinityThread::AffinityThread(const unsigned coreNumber) : core(coreNumber) {}

void AffinityThread::run()
{
  Thread::run();
  this->setAffinity(this->core);
}

void AffinityThread::setAffinity(const unsigned coreNumber)
{
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) ||               \
                         (defined(__APPLE__) && defined(__MACH__)))
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(coreNumber, &cpuset);
  int result = pthread_setaffinity_np(this->getThread().native_handle(),
                                      sizeof(cpu_set_t), &cpuset);
  if (result)
  {
    std::cerr << "ERROR: error set visualization thread affinity, error code: "
              << result << std::endl;
  }
#elif defined(_WIN32) || defined(_WIN64)
  DWORD result = SetAffinityMask(this->getThread().native_handle(),
                                 (1 << this->CORE_NUMBER));
  if (FAILED(result))
  {
    std::cerr << "ERROR: error set visualization thread affinity, error code: "
              << result << std::endl;
  }
#endif
}

} // namespace zge
