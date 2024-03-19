/** @file DG/Core/FileIo.cpp */

#include <DG/Core/FileIo.hpp>

namespace dg
{

  Path FileIo::getAbsolute (const Path& path)
  {
    return fs::absolute(path).lexically_normal();
  }

  Bool FileIo::loadTextFile (const String& filename, const LineFunction& lineFunction)
  {
    if (filename.empty()) {
      DG_ENGINE_ERROR("No text filename specified for loading.");
      return false;
    }

    if (lineFunction == nullptr) {
      DG_ENGINE_ERROR("No line processing function specified.");
      return false;
    }

    if (fs::exists(filename) == false) {
      DG_ENGINE_ERROR("Text file '{}' not found.", filename);
      return false;
    }

    std::fstream file { filename, std::ios::in };
    if (file.is_open() == false) {
      DG_ENGINE_ERROR("Could not open text file '{}' for reading.", filename);
      return false;
    }

    String line = "";
    Index lineNumber = 0;
    while (std::getline(file, line)) {
      lineNumber++;
      if (lineFunction(line, lineNumber) == false) {
        DG_ENGINE_ERROR("Error processing line #{} of text file '{}'.",
          lineNumber, filename);
        return false;
      }
    }

    return true;
  }

  Bool FileIo::loadTextFile (const String& filename, String& contents)
  {
    contents = "";

    return loadTextFile(filename, [&] (StringView line, Index)
    {
      contents += line;
      contents += "\n";
      return true;
    });
  }

  Bool FileIo::saveTextFile (const String& filename, const String& contents)
  {
    if (filename.empty()) {
      DG_ENGINE_ERROR("No text filename specified for saving.");
      return false;
    }

    std::fstream file { filename, std::ios::out };
    if (file.is_open() == false) {
      DG_ENGINE_ERROR("Could not open text file '{}' for writing.", filename);
      return false;
    }

    file << contents << std::endl;
    return true;
  }

}
