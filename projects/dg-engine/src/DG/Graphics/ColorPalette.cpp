/** @file DG/Graphics/ColorPalette.cpp */

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

  ColorPalette::ColorPalette (const Path& filepath)
  {
    loadFromFile(filepath);
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

}
