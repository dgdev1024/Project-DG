/** @file DG/Graphics/Shader.cpp */

#include <DG/Core/FileIo.hpp>
#include <DG/Graphics/Shader.hpp>

namespace dg
{

  Dictionary<Ref<Shader>> ShaderManager::s_assets;

  Shader::Shader ()
  {

  }

  Shader::~Shader ()
  {
    glDeleteProgram(m_handle);
  }

  Ref<Shader> Shader::make (const String& vertexCode, const String& fragmentCode)
  {
    auto shader = makeRef<Shader>();
    shader->loadFromSources(vertexCode, fragmentCode);
    return shader;
  }

  Ref<Shader> Shader::make (const Path& path)
  {
    auto shader = makeRef<Shader>();
    shader->loadFromFile(path);
    return shader;
  }

  void Shader::bind () const
  {
    glUseProgram(m_handle);
  }

  void Shader::unbind ()
  {
    glUseProgram(0);
  }
  
  Boolean Shader::loadFromSources (const String& vertexCode, const String& fragmentCode)
  {
    m_vertexCode = vertexCode;
    m_fragmentCode = fragmentCode;

    return build();
  }

  Boolean Shader::loadFromFile (const Path& path)
  {
    String* codePtr = nullptr;

    Boolean result = FileIo::loadTextFile(
      path,
      [&] (StringView line, Index number)
      {
        if (line.starts_with("#shader ")){
          if (line.ends_with("vertex")) { codePtr = &m_vertexCode; }
          else if (line.ends_with("fragment")) { codePtr = &m_fragmentCode; }
          else {
            DG_ENGINE_ERROR("Invalid #shader directive.");
            return false;
          }
        } else {
          if (codePtr == nullptr) {
            DG_ENGINE_ERROR("No #shader directive set.");
            return false;
          } else {
            *codePtr += line;
            *codePtr += "\n";
          }
        }

        return true;
      }
    );

    return result == true && build();
  }

  #define DG_UNIFORM_LOCATION(type, gl_func, ...) \
    template <> \
    void Shader::setUniform<type> ( \
      const String& name, \
      const type value \
    ) \
    { \
      if (isValid() == false) { \
        DG_ENGINE_CRIT("Attempt to set uniform '{}' on invalid shader!", name); \
        throw std::runtime_error { "Attempt to set uniform on invalid shader!" }; \
      } \
      glUseProgram(m_handle); \
      auto location = findUniform(name); \
      if (location != -1) { \
        gl_func(location, __VA_ARGS__); \
      } \
    }

  DG_UNIFORM_LOCATION(Float32,   glUniform1f, value)
  DG_UNIFORM_LOCATION(Vector2f,  glUniform2f, value.x, value.y)
  DG_UNIFORM_LOCATION(Vector3f,  glUniform3f, value.x, value.y, value.z)
  DG_UNIFORM_LOCATION(Vector4f,  glUniform4f, value.x, value.y, value.z, value.w)
  DG_UNIFORM_LOCATION(Matrix2f,  glUniformMatrix2fv, 1, GL_FALSE, (float*) &value)
  DG_UNIFORM_LOCATION(Matrix3f,  glUniformMatrix3fv, 1, GL_FALSE, (float*) &value)
  DG_UNIFORM_LOCATION(Matrix4f,  glUniformMatrix4fv, 1, GL_FALSE, (float*) &value)

  DG_UNIFORM_LOCATION(Float64,   glUniform1d, value)
  DG_UNIFORM_LOCATION(Vector2d,  glUniform2d, value.x, value.y)
  DG_UNIFORM_LOCATION(Vector3d,  glUniform3d, value.x, value.y, value.z)
  DG_UNIFORM_LOCATION(Vector4d,  glUniform4d, value.x, value.y, value.z, value.w)
  DG_UNIFORM_LOCATION(Matrix2d,  glUniformMatrix2dv, 1, GL_FALSE, (double*) &value)
  DG_UNIFORM_LOCATION(Matrix3d,  glUniformMatrix3dv, 1, GL_FALSE, (double*) &value)
  DG_UNIFORM_LOCATION(Matrix4d,  glUniformMatrix4dv, 1, GL_FALSE, (double*) &value)

  DG_UNIFORM_LOCATION(Int32,     glUniform1i, value)
  DG_UNIFORM_LOCATION(Vector2i,  glUniform2i, value.x, value.y)
  DG_UNIFORM_LOCATION(Vector3i,  glUniform3i, value.x, value.y, value.z)
  DG_UNIFORM_LOCATION(Vector4i,  glUniform4i, value.x, value.y, value.z, value.w)

  DG_UNIFORM_LOCATION(Uint32,    glUniform1ui, value)
  DG_UNIFORM_LOCATION(Vector2u,  glUniform2ui, value.x, value.y)
  DG_UNIFORM_LOCATION(Vector3u,  glUniform3ui, value.x, value.y, value.z)
  DG_UNIFORM_LOCATION(Vector4u,  glUniform4ui, value.x, value.y, value.z, value.w)

  DG_UNIFORM_LOCATION(Boolean,   glUniform1i, value)
  DG_UNIFORM_LOCATION(Vector2b,  glUniform2i, value.x, value.y)
  DG_UNIFORM_LOCATION(Vector3b,  glUniform3i, value.x, value.y, value.z)
  DG_UNIFORM_LOCATION(Vector4b,  glUniform4i, value.x, value.y, value.z, value.w)

  #undef DG_UNIFORM_LOCATION  

  Boolean Shader::isValid () const
  {
    return m_handle != 0;
  }

  Boolean Shader::build ()
  {
    // The length of the status info log string.
    static constexpr Int32 INFO_LOG_LENGTH = 512;

    // Ensure that both vertex and fragment shader code was provided.
    if (m_vertexCode.empty()) {
      DG_ENGINE_ERROR("No vertex shader code provided.");
      return false;
    } else if (m_fragmentCode.empty()) {
      DG_ENGINE_ERROR("No fragment shader code provided.");
      return false;
    }

    // Keep track of a status code and an info log.
    Int32 status = 0;
    char infoLog[INFO_LOG_LENGTH];

    // We will need the C-string forms of the shader's source code strings.
    const char* vertexCode = m_vertexCode.c_str();
    const char* fragmentCode = m_fragmentCode.c_str();

    // Begin by compiling the vertex shader code, then check the compilation
    // status.
    Uint32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(vertexShader, INFO_LOG_LENGTH, nullptr, infoLog);
    if (status != GL_TRUE) {
      DG_ENGINE_ERROR("Error compiling GLSL vertex shader: {}", infoLog);
      glDeleteShader(vertexShader);
      return false;
    } else if (infoLog[0] != '\0') {
      DG_ENGINE_WARN("GLSL vertex shader compiled with warning: {}", infoLog);
    }

    // Repeat this now for the fragment shader.
    Uint32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(fragmentShader, INFO_LOG_LENGTH, nullptr, infoLog);
    if (status != GL_TRUE) {
      DG_ENGINE_ERROR("Error compiling GLSL fragment shader: {}", infoLog);
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);
      return false;
    } else if (infoLog[0] != '\0') {
      DG_ENGINE_WARN("GLSL fragment shader compiled with warning: {}", infoLog);
    }

    // Next, create the shader program and attach the above-compiled shaders
    // to it.
    Uint32 shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // Link the program, then check status.
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    glGetProgramInfoLog(shaderProgram, INFO_LOG_LENGTH, nullptr, infoLog);
    if (status != GL_TRUE) {
      DG_ENGINE_ERROR("Error linking GLSL shader program: {}", infoLog);
      glDeleteProgram(shaderProgram);
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);
      return false;
    } else if (infoLog[0] != '\0') {
      DG_ENGINE_WARN("GLSL shader program linked with warning: {}", infoLog);
    }

    // Delete the shader objects now.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Now that the new shader program has been successfully built, if there was another shader
    // program present, delete that program now.
    if (m_handle != 0) {
      glDeleteProgram(m_handle);
    }

    m_handle = shaderProgram;
    return true; 
  }

  Int32 Shader::findUniform (StringView name)
  {
    if (isValid() == false || name.empty()) { return -1; }
    return glGetUniformLocation(m_handle, name.data());
  }

  Ref<Shader> ShaderManager::getOrEmplace (const String& filename)
  {
    // Ensure that the relative filename string is provided.
    if (filename.empty()) {
      throw std::invalid_argument { "Attempted 'getOrEmplace' with a blank filename string!" };
    }

    // Check to see if the shader asset is present. If so, then return it.
    auto iter = s_assets.find(filename);
    if (iter != s_assets.end()) { return iter->second; }

    // Create the shader. Ensure that it is valid.
    Ref<Shader> shader = Shader::make(FileIo::getAbsolute(filename));
    if (shader->isValid() == false) {
      DG_ENGINE_CRIT("Could not load shader asset file '{}'!", filename);
      throw std::runtime_error { "Could not load shader asset file!" };
    }

    // Emplace, then return the shader.
    s_assets.emplace(filename, shader);
    return shader;
  }

  Boolean ShaderManager::contains (const String& filename)
  {
    // Ensure that the relative filename string is provided.
    if (filename.empty()) {
      throw std::invalid_argument { "Attempted 'contains' with a blank filename string!" };
    }

    return s_assets.contains(filename);
  }

  Ref<Shader> ShaderManager::get (const String& filename)
  {
    if (filename.empty()) {
      throw std::invalid_argument { "Attempted 'get' with blank string filename!" };
    }

    // Ensure the shader asset exists.
    auto iter = s_assets.find(filename);
    if (iter == s_assets.end()) {
      DG_ENGINE_CRIT("Shader asset '{}' not found!", filename);
      throw std::out_of_range { "Shader asset not found!" };
    }

    return iter->second;    
  }

  Ref<Shader> ShaderManager::remove (const String& filename)
  {
    if (filename.empty()) {
      throw std::invalid_argument { "Attempted 'remove' with blank string filename!" };
    }

    // Ensure the shader asset exists.
    auto iter = s_assets.find(filename);
    if (iter == s_assets.end()) {
      DG_ENGINE_CRIT("Shader asset '{}' not found!", filename);
      throw std::out_of_range { "Shader asset not found!" };
    }

    // Get a copy of the shader before removing it.
    auto removed = s_assets.at(filename);
    s_assets.erase(iter);

    return removed;    
  }

  void ShaderManager::clear ()
  {
    s_assets.clear();
  }

}
