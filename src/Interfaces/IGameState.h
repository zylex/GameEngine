#ifndef IGAMESTATE_H
#define IGAMESTATE_H

#include <vector>

#include "Preprocessors.h"

namespace zge
{
// forward declarations
struct Event;
class IGameObject;
class Camera;
class Light;

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
  virtual void cleanAll() = 0;

  virtual void addLight(Light*) = 0;
  virtual std::vector<Light*> getLights() const = 0;
};

} // namespace zge

#endif
