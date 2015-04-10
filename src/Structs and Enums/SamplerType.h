#ifndef SAMPLERTYPE_H
#define SAMPLERTYPE_H

namespace zge
{

enum SamplerType
{
  ZGE_SAMPLER_NULL,
  POINT_WRAP,
  POINT_CLAMP,
  LINEAR_WRAP,
  LINEAR_CLAMP,
  // ANISOTROPIC_WRAP,
  // ANISOTROPIC_CLAMP,
  TOTAL_SAMPLERS
};

} // namespace zge

#endif