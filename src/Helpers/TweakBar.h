#ifdef USE_ANT
#ifndef TWEAKBAR_H
#define TWEAKBAR_H

#include <string>
#include <AntTweakBar.h>
#include <glm/vec2.hpp>

#include "Preprocessors.h"

namespace zge
{

class TweakBar
{
private:
  const char* barName;
  std::string barTitle;
  glm::uvec2 position;
  glm::uvec2 size;
  TwBar* antTweakBar;
  static unsigned barNumber;

public:
  TweakBar();
  TweakBar(std::string barTitle);
  virtual ~TweakBar() NOEXCEPT;

  void init();

  virtual void initialise();

  TwBar* getAntTweakBar() const;
  const char* getTweakBarName() const;
};

} // namespace zge

#endif
#endif
