/** @file DG/Core/LayerStack.cpp */

#include <DG/Core/LayerStack.hpp>

namespace dg
{

  LayerStack::LayerStack ()
  {
    
  }

  LayerStack::~LayerStack ()
  {
    for (auto layerPtr : m_layerPtrs) {
      if (layerPtr != nullptr)
        layerPtr->onDetach();
    }

    m_layerPtrs.clear();
  }

  void LayerStack::attachLayer (Layer& layer)
  {
    // Ensure that the layer has not already been attached.
    auto iter = std::find(m_layerPtrs.begin(), m_layerPtrs.end(), &layer);
    if (iter != m_layerPtrs.end()) {
      DG_ENGINE_WARN("Layer '{}' has already been attached.", layer.getName());
      return;
    }

    // Determine where we need to attach the layer.
    if (layer.isOverlay() == true) {
      m_layerPtrs.emplace_back(&layer);
    } else {
      m_layerPtrs.emplace(m_layerPtrs.begin() + m_layerInsertIndex, &layer);
      m_layerInsertIndex++;
    }

    layer.onAttach();
  }

  void LayerStack::detachLayer (Layer& layer)
  {
    // Ensure that the layer is attached.
    auto iter = std::find(m_layerPtrs.begin(), m_layerPtrs.end(), &layer);
    if (iter == m_layerPtrs.end()) {
      DG_ENGINE_ERROR("Layer '{}' is not currently attached.", layer.getName());
      return;
    }

    // Detach the layer.
    layer.onDetach();
    m_layerPtrs.erase(iter);

    if (layer.isOverlay() == false) {
      m_layerInsertIndex--;
    }
  }

}
