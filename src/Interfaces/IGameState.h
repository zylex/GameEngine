#ifndef IGAMESTATE_H
#define IGAMESTATE_H

namespace zge {

struct Event;
class IRenderer;
class IGameObject;
class Light;
class Camera;

class IGameState {
public:
  virtual void processEvent(const Event) const = 0;

  virtual bool initialise(IRenderer*) = 0;
  virtual void update() = 0;

  virtual void setLight(Light* light) = 0;
  virtual Light* getLight() = 0;
  virtual void setCurrentCamera(Camera* camera) = 0;
  virtual Camera* getCurrentCamera() = 0;
  virtual void addGameObject(IGameObject* gameObject) = 0;

  virtual void draw() = 0;
};

} // namespace zge

#endif