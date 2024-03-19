/** @file DG/Core/FileLexer.cpp */

#include <DG/Core/FileLexer.hpp>

namespace dg
{

  namespace Private
  {

    // Helper function for collecting a symbol token.
    static Bool collectSymbol (FileToken& token, const Int32& character)
    {
      switch (character) {
        case '.': token.type = FileTokenType::Period; break;
        case ':': token.type = FileTokenType::Colon; break;
        case ';': token.type = FileTokenType::Semicolon; break;
        case ',': token.type = FileTokenType::Comma; break;
        case '#': token.type = FileTokenType::Pound; break;
        case '@': token.type = FileTokenType::At; break;
        case '=': token.type = FileTokenType::Equals; break;
        case '+': token.type = FileTokenType::Plus; break;
        case '-': token.type = FileTokenType::Minus; break;
        case '*': token.type = FileTokenType::Times; break;
        case '/': token.type = FileTokenType::Divide; break;
        case '%': token.type = FileTokenType::Percent; break;
        case '|': token.type = FileTokenType::Pipe; break;
        case '&': token.type = FileTokenType::Ampersand; break;
        case '^': token.type = FileTokenType::Carat; break;
        case '(': token.type = FileTokenType::OpenParenthesis; break;
        case ')': token.type = FileTokenType::CloseParenthesis; break;
        case '[': token.type = FileTokenType::OpenSquareBracket; break;
        case ']': token.type = FileTokenType::CloseSquareBracket; break;
        case '{': token.type = FileTokenType::OpenCurlyBrace; break;
        case '}': token.type = FileTokenType::CloseCurlyBrace; break;
        case '<': token.type = FileTokenType::OpenArrowBrace; break;
        case '>': token.type = FileTokenType::CloseArrowBrace; break;
        default:
          DG_ENGINE_ERROR("Unexpected character '%c'.", static_cast<Char>(character));
          return false;
      }

      return true;
    }

    // Helper function for collecting a binary number.
    static Bool collectBinaryNumber (std::fstream& file, FileToken& token, Int32& character)
    {
      // Clear the token's string contents.
      token.contents.clear();

      // Advance the file stream past the percent sign.
      character = file.get();

      // Loop until a non-binary character is found.
      while (character == '0' || character == '1') {
        token.contents += static_cast<Char>(character);
        character = file.get();
      }

      // Move the file stream back one place.
      file.unget();

      token.type = (token.contents.empty() == true) ? FileTokenType::Percent :
        FileTokenType::Binary;
      return true;
    }

    // Helper function for collecting an octal number.
    static Bool collectOctalNumber (std::fstream& file, FileToken& token, Int32& character)
    {
      // Clear the token's string contents.
      token.contents.clear();

      // Advance the file stream past the ampersand sign.
      character = file.get();

      // Loop until a non-octal character is found.
      while (character >= '0' || character <= '7') {
        token.contents += static_cast<Char>(character);
        character = file.get();
      }

      // Move the file stream back one place.
      file.unget();

      token.type = (token.contents.empty() == true) ? FileTokenType::Ampersand :
        FileTokenType::Octal;
      return true;
    }

    // Helper function for collecting a hexadecimal number.
    static Bool collectHexadecimalNumber (std::fstream& file, FileToken& token, Int32& character)
    {
      // Clear the token's string contents.
      token.contents.clear();

      // Advance the file stream past the dollar sign.
      character = file.get();

      // Loop until a non-binary character is found.
      while (std::isxdigit(character)) {
        token.contents += static_cast<Char>(character);
        character = file.get();
      }

      // Move the file stream back one place.
      file.unget();

      token.type = (token.contents.empty() == true) ? FileTokenType::DollarSign :
        FileTokenType::Hexadecimal;
      return true;
    }

    static Bool collectNumericToken (std::fstream& file, FileToken& token, Int32& character)
    {
      // Keep track of whether or not this numeric token is an integer or a floating point number.
      Bool isFloat = false;

      // Clear the token's string contents.
      token.contents.clear();

      // Loop until a non-numeric character which is not the first period is encountered.
      do {
        if (character == '.') {
          if (isFloat == true) { break; }
          isFloat = true;
        }

        token.contents += static_cast<Char>(character);
        character = file.get();
      } while (std::isdigit(character) || character == '.');

      // Move the file stream back one place.
      file.unget();

      token.type = (isFloat == true) ? FileTokenType::FloatingPoint : FileTokenType::Integer;
      return true;
    }

    static Bool collectStringToken (std::fstream& file, FileToken& token, Int32& character)
    {
      // Clear the token's string contents.
      token.contents.clear();

      // Advance the file stream past the opening quote.
      character = file.get();

      // Loop until a non-binary character is found.
      while (character != '"') {
        token.contents += static_cast<Char>(character);
        character = file.get();
      }

      token.type = FileTokenType::String;
      return true;
    }

    static Bool collectIdentifierToken (std::fstream& file, FileToken& token, Int32& character)
    {
      // Clear the token's string contents.
      token.contents.clear();

      // Loop until a non-numeric character which is not the first period is encountered.
      do {
        token.contents += static_cast<Char>(character);
        character = file.get();
      } while (std::isalnum(character) || character == '_');

      // Move the file stream back one place.
      file.unget();

      if (token.contents == "true" || token.contents == "false") {
        token.type = FileTokenType::Boolean;
      } else {
        token.type = FileTokenType::Identifier;
      }

      return true;
    }

    // Helper function for collecting the next token in the current file.
    static Bool collectToken (std::fstream& file, FileToken& token)
    {

      // Get the next character in the file stream.
      Int32 character = file.get();                 // Should be the same as C's `fgetc(FILE*)`.

      // As long as the character encountered is a whitespace character...
      while (std::isspace(character)) {
        
        // If that whitespace character is the newline character, then set up the token as such.
        if (character == '\n') {
          token.type = FileTokenType::NewLine;
          return true;
        }

        // Skip over the whitespace character.
        character = file.get();

      }

      // Is this character the End Of File marker (EOF)?
      if (character == std::char_traits<Char>::eof()) {
        token.type = FileTokenType::EndOfFile;
        return true;
      }

      // Now check the type of token that needs to be extracted.
      if (std::isalpha(character) || character == '_') {
        return collectIdentifierToken(file, token, character);
      } else if (character == '"') {
        return collectStringToken(file, token, character);
      } else if (std::isdigit(character)) {
        return collectNumericToken(file, token, character);
      } else if (character == '$') {
        return collectHexadecimalNumber(file, token, character);
      } else if (character == '%') {
        return collectBinaryNumber(file, token, character);
      } else if (character == '&') {
        return collectOctalNumber(file, token, character);  
      } else {
        return collectSymbol(token, character);
      }

    }

  }

  FileLexer::FileLexer ()
  {

  }

  FileLexer::FileLexer (const Path& filepath)
  {
    loadFromFile(filepath);
  }

  Bool FileLexer::loadFromFile (const Path& filepath)
  {

    // Ensure that the file exists.
    if (fs::exists(filepath) == false) {
      DG_ENGINE_ERROR("Text file '{}' not found.", filepath);
      return false;
    }

    // Don't lex a file which has already been lexed!
    Path absolute = fs::absolute(filepath).lexically_normal();
    if (std::find(m_includes.begin(), m_includes.end(), absolute) != m_includes.end()) {
      return true;
    }

    // Attempt to open the file.
    std::fstream file { filepath, std::ios::in };
    if (file.is_open() == false) {
      DG_ENGINE_ERROR("Could not open text file '{}' for reading.", filepath);
      return false;
    }

    // Keep track of the current file token.
    FileToken token;
    token.sourceFile = filepath;
    token.sourceLine = 1;

    // Also keep track of the current token position.
    Index tokenPointer = m_tokenPointer;

    // Loop as long as there are tokens left to parse.
    while (true) {

      // Parse the next token.
      if (Private::collectToken(file, token) == false) {
        DG_ENGINE_ERROR("Parse error in text file '{}:{}'.", token.sourceFile, token.sourceLine);
        return false;
      }

      // Insert the token into the collection.
      m_tokens.insert(m_tokens.begin() + tokenPointer, token);
      tokenPointer++;

      // If this token was the End of File token, then break.
      if (token.type == FileTokenType::EndOfFile) {
        break;
      }

      // If this token was a New Line token, then increment the line number.
      else if (token.type == FileTokenType::NewLine) {
        token.sourceLine++;
      }

    }

    // Mark the file as lexed.
    m_includes.push_back(absolute);

    return true;

  }

  Bool FileLexer::hasMoreTokens () const
  {
    return m_tokens.empty() == false && m_tokenPointer < m_tokens.size();
  }

  const FileToken& FileLexer::getNextToken () const
  {
    if (m_tokenPointer >= m_tokens.size()) { return m_tokens.back(); }
    else { return m_tokens.at(m_tokenPointer++); }
  }

  void FileLexer::rewind () const
  {
    m_tokenPointer = 0;
  }

}
