#include "Thread.h"

namespace zge
{

void Thread::run() { this->theThread = std::thread(&Thread::cb, this); }

void Thread::join() { this->theThread.join(); }

std::thread& Thread::getThread() { return this->theThread; }

void Thread::cb() { this->callback(); }

} // namespace zge
