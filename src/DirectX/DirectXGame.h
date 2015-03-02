#ifndef DIRECT_X_GAME_H
#define DIRECT_X_GAME_H

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include <string>

namespace zge
{
namespace dx
{
class DirectXGame
{
public:
  DirectXGame(std::string);
};
}
}
#endif
