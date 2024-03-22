/** @file DG/Graphics/ColorPalette.cpp */

#include <DG/Core/FileIo.hpp>
#include <DG/Core/FileLexer.hpp>
#include <DG/Graphics/ColorPalette.hpp>

namespace dg
{

  namespace Private
  {

    static Bool parseCommandRGBA (ColorPalette& palette, FileLexer& lexer)
    {
      while (true) {

        FileToken paramOne      = lexer.getNextToken();
        Bool      hasCommaOne   = lexer.getNextToken().type == FileTokenType::Comma;
        FileToken paramTwo      = lexer.getNextToken();
        Bool      hasCommaTwo   = lexer.getNextToken().type == FileTokenType::Comma;
        FileToken paramThree    = lexer.getNextToken();
        Bool      hasCommaThree = lexer.getNextToken().type == FileTokenType::Comma;
        FileToken paramFour     = lexer.getNextToken();
        Bool      commandOver   = lexer.getNextToken(false).type != FileTokenType::Comma;

        if (hasCommaOne == false || hasCommaTwo == false || hasCommaThree == false) {
          DG_ENGINE_ERROR("Missing ',' from parameters to 'RGBA'.");
          return false;
        } else if (
          paramOne.type != FileTokenType::Integer ||
          paramTwo.type != FileTokenType::Integer ||
          paramThree.type != FileTokenType::Integer ||
          paramFour.type != FileTokenType::Integer
        ) {
          DG_ENGINE_ERROR("Parameter type mismatch in call to 'RGBA'.");
          return false;
        }

        palette.pushColor(Color::fromIntegers(
          std::stoi(paramOne.contents),
          std::stoi(paramTwo.contents),
          std::stoi(paramThree.contents),
          std::stoi(paramFour.contents)
        ));

        if (commandOver == true) { break; }
        else { lexer.getNextToken(); }

      }

      return true;
    }

    static Bool parseCommandRGB (ColorPalette& palette, FileLexer& lexer)
    {
      while (true) {

        FileToken paramOne    = lexer.getNextToken();
        Bool      hasCommaOne = lexer.getNextToken().type == FileTokenType::Comma;
        FileToken paramTwo    = lexer.getNextToken();
        Bool      hasCommaTwo = lexer.getNextToken().type == FileTokenType::Comma;
        FileToken paramThree  = lexer.getNextToken();
        Bool      commandOver = lexer.getNextToken(false).type != FileTokenType::Comma;

        if (hasCommaOne == false || hasCommaTwo == false) {
          DG_ENGINE_ERROR("Missing ',' from parameters to 'RGB'.");
          return false;
        } else if (
          paramOne.type != FileTokenType::Integer ||
          paramTwo.type != FileTokenType::Integer ||
          paramThree.type != FileTokenType::Integer
        ) {
          DG_ENGINE_ERROR("Parameter type mismatch in call to 'RGB'.");
          return false;
        }

        palette.pushColor(Color::fromIntegers(
          std::stoi(paramOne.contents),
          std::stoi(paramTwo.contents),
          std::stoi(paramThree.contents)
        ));

        if (commandOver == true) { break; }
        else { lexer.getNextToken(); }

      }

      return true;
    }

    static Bool parsePaletteFile (ColorPalette& palette, FileLexer& lexer)
    {
      FileToken token;

      while (lexer.hasMoreTokens() == true) {
        token = lexer.getNextToken();

        if (token.type == FileTokenType::Semicolon) {
          while (token.type != FileTokenType::NewLine && token.type != FileTokenType::EndOfFile) {
            token = lexer.getNextToken();
          }

          continue;
        }

        if (token.type == FileTokenType::NewLine) {
          continue;
        }

        if (token.type == FileTokenType::EndOfFile) {
          break;
        }

        if (token.type != FileTokenType::Identifier) {
          DG_ENGINE_ERROR("Unexpected '{}' token in line #{}.", token.typeToString(),
            token.sourceLine);
          return false;
        }

        if (token.contents == "RGB") {
          if (parseCommandRGB(palette, lexer) == false) {
            DG_ENGINE_ERROR("Error parsing 'RGB' command in line #{}.", token.sourceLine);
            return false;
          }
        } else if (token.contents == "RGBA") {
          if (parseCommandRGBA(palette, lexer) == false) {
            DG_ENGINE_ERROR("Error parsing 'RGBA' command in line #{}.", token.sourceLine);
            return false;
          }
        } else {
          DG_ENGINE_ERROR("Unexpected '{}' token ('{}') in line #{}.", token.typeToString(),
            token.contents, token.sourceLine);
          return false;
        }
      }

      return true;
    }

  }

  ColorPalette::ColorPalette ()
  {

  }

  Ref<ColorPalette> ColorPalette::make ()
  {
    return makeRef<ColorPalette>();
  }

  Ref<ColorPalette> ColorPalette::make (const Path& filepath)
  {
    auto palette = makeRef<ColorPalette>();
    palette->loadFromFile(filepath);
    return palette;
  }

  Bool ColorPalette::loadFromFile (const Path& filepath)
  {

    // Ensure that the palette file exists.
    if (fs::exists(filepath) == false) {
      DG_ENGINE_ERROR("Color palette file '{}' not found.", filepath);
      return false;
    }

    // Extract syntax tokens from the file.
    FileLexer lexer { filepath };
    if (lexer.isEmpty() == true) {
      DG_ENGINE_ERROR("Color palette file '{}' is either empty or was not loaded correctly.",
        filepath);
      return false;
    }

    // Parse the tokens.
    if (Private::parsePaletteFile(*this, lexer) == false) {
      DG_ENGINE_ERROR("  While parsing palette file '{}'.", filepath);
      return false;
    }

    return true;
  }

  void ColorPalette::pushColor (const Color& color)
  {
    m_colors.push_back(color);
  }

  Bool ColorPalette::isEmpty () const
  {
    return m_colors.empty();
  }

  Count ColorPalette::getColorCount () const
  {
    return m_colors.size();
  }

  Color& ColorPalette::get (const Index index)
  {
    if (index >= m_colors.size()) {
      DG_ENGINE_CRIT("Color palette index #{} is out of range!", index);
      throw std::out_of_range { "Color palette index is out of range!" };
    }

    return m_colors.at(index);
  }

  const Color& ColorPalette::get (const Index index) const
  {
    if (index >= m_colors.size()) {
      DG_ENGINE_CRIT("Color palette index #{} is out of range!", index);
      throw std::out_of_range { "Color palette index is out of range!" };
    }

    return m_colors.at(index);
  }

  Color& ColorPalette::operator[] (const Index index)
  {
    if (index >= m_colors.size()) {
      DG_ENGINE_CRIT("Color palette index #{} is out of range!", index);
      throw std::out_of_range { "Color palette index is out of range!" };
    }

    return m_colors.at(index);
  }

  const Color& ColorPalette::operator[] (const Index index) const
  {
    if (index >= m_colors.size()) {
      DG_ENGINE_CRIT("Color palette index #{} is out of range!", index);
      throw std::out_of_range { "Color palette index is out of range!" };
    }

    return m_colors.at(index);
  }

  Dictionary<Ref<ColorPalette>> ColorPaletteManager::s_assets;

  Ref<ColorPalette> ColorPaletteManager::getOrEmplace (const String& filename)
  {
    // Ensure that the relative filename string is provided.
    if (filename.empty()) {
      throw std::invalid_argument { "Attempted 'getOrEmplace' with a blank filename string!" };
    }

    // Check to see if the palette asset is present. If so, then return it.
    auto iter = s_assets.find(filename);
    if (iter != s_assets.end()) { return iter->second; }

    // Create the palette. Ensure that it is valid.
    Ref<ColorPalette> palette = ColorPalette::make(FileIo::getAbsolute(filename));
    if (palette->isEmpty() == false) {
      DG_ENGINE_CRIT("Could not load palette asset file '{}'!", filename);
      throw std::runtime_error { "Could not load palette asset file!" };
    }

    // Emplace, then return the palette.
    s_assets.emplace(filename, palette);
    return palette;
  }

  Boolean ColorPaletteManager::contains (const String& filename)
  {
    // Ensure that the relative filename string is provided.
    if (filename.empty()) {
      throw std::invalid_argument { "Attempted 'contains' with a blank filename string!" };
    }

    return s_assets.contains(filename);
  }

  Ref<ColorPalette> ColorPaletteManager::get (const String& filename)
  {
    if (filename.empty()) {
      throw std::invalid_argument { "Attempted 'get' with blank string filename!" };
    }

    // Ensure the palette asset exists.
    auto iter = s_assets.find(filename);
    if (iter == s_assets.end()) {
      DG_ENGINE_CRIT("ColorPalette asset '{}' not found!", filename);
      throw std::out_of_range { "ColorPalette asset not found!" };
    }

    return iter->second;    
  }

  Ref<ColorPalette> ColorPaletteManager::remove (const String& filename)
  {
    if (filename.empty()) {
      throw std::invalid_argument { "Attempted 'remove' with blank string filename!" };
    }

    // Ensure the palette asset exists.
    auto iter = s_assets.find(filename);
    if (iter == s_assets.end()) {
      DG_ENGINE_CRIT("ColorPalette asset '{}' not found!", filename);
      throw std::out_of_range { "ColorPalette asset not found!" };
    }

    // Get a copy of the palette before removing it.
    auto removed = s_assets.at(filename);
    s_assets.erase(iter);

    return removed;    
  }

  void ColorPaletteManager::clear ()
  {
    s_assets.clear();
  }

}
