/** @file DGStudio/Main.cpp */

#include <DGStudio/StudioApplication.hpp>

int main ()
{
  dg::ApplicationSpecification spec;
  spec.guiSpec.enabled = true;
  spec.guiSpec.docking = true;

  dgstudio::StudioApplication app { spec }; 
  return app.start();
}
