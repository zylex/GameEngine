#include <iostream>
#include <algorithm>

#include "InstancedShader.h"

#include "MeshObject.h"

namespace zge
{

template <typename I, typename D, typename F, typename S>
InstancedShader<I, D, F, S>::InstancedShader()
{
  // constructor
}

template <typename I, typename D, typename F, typename S>
InstancedShader<I, D, F, S>::~InstancedShader() NOEXCEPT
{
  // destructor
}

template <typename I, typename D, typename F, typename S>
void InstancedShader<I, D, F, S>::addInstance(IGameObject* iInstance)
{
  MeshObject* instance = static_cast<MeshObject*>(iInstance);
  if (instance IS nullptr)
  {
    std::cerr << "Instance is not a MeshObject, not adding to shader."
              << std::endl;
    return;
  }

  D data = this->getInstanceData(instance);

  typename std::vector<I>::iterator instanceEnd, instanceBegin,
      instanceIterator;
  instanceEnd = this->instances.end();
  instanceBegin = this->instances.begin();
  instanceIterator = std::find_if(instanceBegin, instanceEnd, F(instance));

  if (instanceIterator IS instanceEnd) // not already there
  {
    this->instances.push_back(this->getInstanceUnit(instance, data));
    std::sort(instanceBegin, instanceEnd, S());
  }
  else
  {
    std::vector<MeshObject*> instanceObjects =
        instanceIterator->instanceObjects;
    std::vector<MeshObject*>::iterator instanceObjectsFound, instanceObjectsEnd;
    instanceObjectsEnd = instanceObjects.end();
    instanceObjectsFound =
        std::find(instanceObjects.begin(), instanceObjectsEnd, instance);
    if (instanceObjectsFound IS instanceObjectsEnd) // not already there
    {
      instanceIterator->instanceObjects.push_back(instance);
      instanceIterator->instanceData.push_back(data);
    }
    // else
    // {
    //   // do nothing
    // }
  }
}

template <typename I, typename D, typename F, typename S>
void InstancedShader<I, D, F, S>::clear()
{
  this->instances.clear();
}

template <typename I, typename D, typename F, typename S>
void InstancedShader<I, D, F, S>::prepare()
{
  ShaderProgram::prepare();
}

template <typename I, typename D, typename F, typename S>
void InstancedShader<I, D, F, S>::render()
{
  typename std::vector<I>::iterator instanceEnd, instanceBegin,
      instanceIterator;
  instanceEnd = this->instances.end();
  for (instanceIterator = this->instances.begin();
       instanceIterator IS_NOT instanceEnd; ++instanceIterator)
  {
    this->updateData(instanceIterator->instanceObjects,
                     instanceIterator->instanceData);
    this->executeShader(*instanceIterator);
  }
}

template <typename I, typename D, typename F, typename S>
void InstancedShader<I, D, F, S>::updateData(
    std::vector<MeshObject*>& instanceObjects, std::vector<D>& instanceData)
{
  std::vector<MeshObject*>::iterator dataIterator, dataBegin, dataEnd;
  dataBegin = instanceObjects.begin();
  dataEnd = instanceObjects.end();
  for (dataIterator = dataBegin; dataIterator IS_NOT dataEnd; ++dataIterator)
  {
    // if ((*dataIterator)->isDirty())
    // {
    unsigned long index = std::distance(dataBegin, dataIterator);
    instanceData[index] = this->getInstanceData((*dataIterator));
    // TODO: implement a way to check if it's the last shader to use it
    // TODO: alternative is to use observer pattern
    // (*dataIterator)->clean();
    // }
  }
}

} // namespace zge
