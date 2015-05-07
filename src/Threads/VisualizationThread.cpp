#include "VisualizationThread.h"

#include "IGame.h"
#include "IRenderer.h"

namespace zge
{
VisualizationThread::VisualizationThread()
    : AffinityThread(VisualizationThread::CORE_NUMBER),
      game(IGame::getInstance()),
      renderer(IRenderer::getInstance())
{
}

VisualizationThread::~VisualizationThread() NOEXCEPT { this->join(); }

void VisualizationThread::callback()
{
  while (this->game->isRunning())
  {
    this->renderer->render();
  }
}

} // namespace zge
