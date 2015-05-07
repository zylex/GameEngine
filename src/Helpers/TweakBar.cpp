#ifdef USE_ANT
#include <sstream>

#include "TweakBar.h"

#include "Stats.h"
#include "Game.h"

namespace zge
{

unsigned TweakBar::barNumber = 0;

TweakBar::TweakBar() : TweakBar("") {}

TweakBar::TweakBar(std::string barTitle)
    : barTitle(barTitle),
      position({ 0, 0 }),
      size({ 240, 600 }),
      antTweakBar(nullptr)
{
  this->init();
}

TweakBar::~TweakBar() NOEXCEPT
{
  TwDeleteBar(this->antTweakBar);
  this->antTweakBar = nullptr;
}

void TweakBar::init()
{
  TwWindowSize(SCREEN_WIDTH(), SCREEN_HEIGHT());
  std::ostringstream stringStream;
  stringStream << "bar" << TweakBar::barNumber;
  ++TweakBar::barNumber;
  this->barName = stringStream.str().c_str();
  this->antTweakBar = TwNewBar(this->barName);
  stringStream.str(std::string());
  stringStream << this->barName;
  stringStream << " label='" << this->barTitle << "' size='" << this->size.x
               << " " << this->size.y << "' position='" << this->position.x
               << " " << this->position.y << "' ";
  TwDefine(stringStream.str().c_str());

  TwAddVarRO(this->antTweakBar, "nb_vertices", TW_TYPE_UINT32,
             &(Stats::numberOfVertices),
             "group=Graphics label='Number Of Vertices'");
  TwAddVarRO(this->antTweakBar, "nb_triangles", TW_TYPE_UINT32,
             &(Stats::numberOfTriangles),
             "group=Graphics label='Number Of Triangles'");
  TwAddVarRO(this->antTweakBar, "nb_instances", TW_TYPE_UINT32,
             &(Stats::numberOfInstances),
             "group=Graphics label='Number Of Instances'");
  TwAddSeparator(this->antTweakBar, nullptr, "");
  stringStream.str(std::string());
  stringStream << this->barName << "/Graphics opened=false";
  TwDefine(stringStream.str().c_str());

  TwAddVarRO(this->antTweakBar, "main_fps", TW_TYPE_FLOAT,
             &(Stats::getInstance()->getMainFPS()),
             "precision=2 group=Main label='FPS'");
  stringStream.str(std::string());
  stringStream << this->barName << "/Main opened=false";
  TwDefine(stringStream.str().c_str());

  TwAddVarRO(this->antTweakBar, "visualization_fps", TW_TYPE_FLOAT,
             &Stats::getInstance()->getVisualizationFPS(),
             "precision=2 group=Visualization label='FPS'");
  stringStream.str(std::string());
  stringStream << this->barName << "/Visualization opened=false";
  TwDefine(stringStream.str().c_str());

  TwAddVarRO(this->antTweakBar, "network_fps", TW_TYPE_FLOAT,
             &Stats::getInstance()->getNetworkFPS(),
             "precision=2 group=Network label='FPS'");
  stringStream.str(std::string());
  stringStream << this->barName << "/Network opened=false";
  TwDefine(stringStream.str().c_str());

  TwAddVarRO(this->antTweakBar, "simulation_fps", TW_TYPE_FLOAT,
             &Stats::getInstance()->getSimulationFPS(),
             "precision=2 group=Simulation label='FPS'");
  stringStream.str(std::string());
  stringStream << this->barName << "/Simulation opened=false";
  TwDefine(stringStream.str().c_str());

  stringStream.str(std::string());
  stringStream << this->barName << "/Main group='Threads'";
  TwDefine(stringStream.str().c_str());
  TwAddSeparator(this->antTweakBar, nullptr, "group='Threads'");
  stringStream.str(std::string());
  stringStream << this->barName << "/Visualization group='Threads'";
  TwDefine(stringStream.str().c_str());
  TwAddSeparator(this->antTweakBar, nullptr, "group='Threads'");
  stringStream.str(std::string());
  stringStream << this->barName << "/Network group='Threads'";
  TwDefine(stringStream.str().c_str());
  TwAddSeparator(this->antTweakBar, nullptr, "group='Threads'");
  stringStream.str(std::string());
  stringStream << this->barName << "/Simulation group='Threads'";
  TwDefine(stringStream.str().c_str());
  TwAddSeparator(this->antTweakBar, nullptr, "group='Threads'");
  stringStream.str(std::string());
  stringStream << this->barName << "/Threads opened=false";
  TwDefine(stringStream.str().c_str());
  this->initialise();
}

void TweakBar::initialise()
{
  // NOTE: Override to add info to the TweakBar
}

TwBar* TweakBar::getAntTweakBar() const { return this->antTweakBar; }

const char* TweakBar::getTweakBarName() const { return this->barName; }

} // namespace zge

#endif
