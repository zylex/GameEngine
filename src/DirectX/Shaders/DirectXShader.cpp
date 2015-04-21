#ifdef DIRECT_X
#include <fstream>

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

#include "Preprocessors.h"

#include "DirectXShader.h"

namespace zge
{

namespace dx
{

const void* CompileShader(const std::string filepath, size_t& size)
{
  // check file extension
  std::string fileSuffix = "S.hlsl";
  if (filepath.length() < fileSuffix.length() and
      not std::equal(fileSuffix.rbegin(), fileSuffix.rend(), filepath.rbegin()))
  {
    return nullptr;
  }
  std::string shaderType = "";
  switch (filepath[filepath.length() - 7])
  {
    case 'C':
      shaderType = "cs_5_0";
      break;
    case 'G':
      shaderType = "gs_5_0";
      break;
    case 'V':
      shaderType = "vs_5_0";
      break;
    case 'P':
      shaderType = "ps_5_0";
      break;
    default:
      return nullptr;
  }
  std::wstring stemp = std::wstring(filepath.begin(), filepath.end());
  ID3DBlob* shaderBlob = nullptr;
  ID3DBlob* errorBlob = nullptr;
  if (FAILED(D3DCompileFromFile(
          stemp.c_str(), nullptr, nullptr, "main", shaderType.c_str(),
          D3DCOMPILE_ENABLE_STRICTNESS, 0, &shaderBlob, &errorBlob)))
  {
    // maybe output an error?
    char* compileErrors;
    unsigned long bufferSize, i;
    std::ofstream fout;

    // Get a pointer to the error message text buffer.
    compileErrors = (char*)(errorBlob->GetBufferPointer());

    // Get the length of the message.
    bufferSize = errorBlob->GetBufferSize();

    // Open a file to write the error message to.
    fout.open("shader-error.txt");

    // Write out the error message.
    for (i = 0; i < bufferSize; i++)
    {
      fout << compileErrors[i];
    }

    // Close the file.
    fout.close();
    return nullptr;
  }

  size = shaderBlob->GetBufferSize();

  return shaderBlob->GetBufferPointer();
}

} // namespace dx

} // namespace zge

#endif
