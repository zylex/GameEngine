#ifndef IGAMESTATE_H
#define IGAMESTATE_H

#include "Preprocessors.h"

namespace zge
{
// forward declarations
struct Event;
class IGameObject;
class Camera;

class IGameState
{
public:
  virtual ~IGameState() NOEXCEPT = default;
  virtual void processEvent(const Event) = 0;

  virtual bool initialise() = 0;
  virtual void update() = 0;

  virtual void setCurrentCamera(Camera* camera) = 0;
  virtual Camera* getCurrentCamera() const = 0;
  virtual void addGameObject(const unsigned shaderId,
                             IGameObject* gameObject) = 0;
};

} // namespace zge

#endif