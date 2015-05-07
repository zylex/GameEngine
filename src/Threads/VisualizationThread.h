#ifndef VISUALIZATIONTHREAD_H
#define VISUALIZATIONTHREAD_H

#include "AffinityThread.h"

namespace zge
{
class IGame;
class IRenderer;

class VisualizationThread : public AffinityThread
{
private:
  static const unsigned CORE_NUMBER = 1;
  IGame* game;
  IRenderer* renderer;

public:
  VisualizationThread();
  virtual ~VisualizationThread() NOEXCEPT;

  void callback();
};

} // namespace zge

#endif
