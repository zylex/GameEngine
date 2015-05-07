#include "Timer.h"

#include "IGlobalTimer.h"

namespace zge
{

Timer::Timer() : endTime(0)
{
  // constructor
  this->globaltimer = IGlobalTimer::getInstance();
  this->startTime = this->globaltimer->getElapsedNanoSeconds();
}

Timer::~Timer() NOEXCEPT
{
  // destructor
  this->globaltimer = nullptr;
}

Timer::Timer(const Timer& other)
{
  // copy constructor
}

Timer& Timer::operator=(const Timer& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

Timer::Timer(Timer&& other) NOEXCEPT
{
  // move constructor (C++11)
}

Timer& Timer::operator=(Timer&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new Timer(other));
}

void Timer::stop()
{
  this->endTime = this->globaltimer->getElapsedNanoSeconds();
}

long long Timer::reset()
{
  this->endTime = this->startTime;
  this->startTime = this->globaltimer->getElapsedNanoSeconds();
  return this->startTime - this->endTime;
}

long long Timer::getElapsedTime()
{
  return this->globaltimer->getElapsedNanoSeconds() - this->startTime;
}

long long Timer::getDeltaTime() { return this->endTime - this->startTime; }

} // namespace zge
