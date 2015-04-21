#include "FileHelper.h"

#include <string>
#include <fstream>
#include <iostream>

namespace zge
{

const char* LoadFile(const std::string filepath)
{
  std::string contents = "";
  std::ifstream filestream(filepath);
  if (filestream.is_open())
  {
    std::string line = "";
    while (std::getline(filestream, line))
    {
      contents += "\n" + line;
    }
  }
  else
  {
    std::cerr << "Could not find the file path: " << filepath << std::endl;
  }

  return (new std::string(contents))->c_str();
}

} // namespace zge
