/** @file DG/Core/GuiContext.cpp */

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <DG/Core/Application.hpp>
#include <DG/Core/GuiContext.hpp>

namespace dg
{

  GuiContext::GuiContext (const GuiContextSpecification& spec) :
    m_docking { spec.docking },
    m_viewport { spec.viewport }
  {
    // Set up the Dear ImGui context...
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Set up the ImGui input events...
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Enable docking and viewport features if requested.
    if (m_docking == true) {
      io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }

    if (m_viewport == true) {
      io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    }

    // Set up the styles.
    ImGui::StyleColorsDark();  

    // Set up the rendering backends.
    ImGui_ImplGlfw_InitForOpenGL(
      Application::getWindow().getPointer(), 
      true
    );
    ImGui_ImplOpenGL3_Init("#version 450 core");
  }

  GuiContext::~GuiContext ()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  Scope<GuiContext> GuiContext::make (const GuiContextSpecification& spec)
  {
    if (spec.enabled == false) {
      return nullptr;
    }

    return makeScope<GuiContext>(spec);
  }

  void GuiContext::begin () const
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  void GuiContext::end () const
  {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (m_viewport == true) {
      GLFWwindow* context = glfwGetCurrentContext();

      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();

      glfwMakeContextCurrent(context);
    }
  }

}
